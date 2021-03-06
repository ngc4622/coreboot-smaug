/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2008-2009 coresystems GmbH
 * Copyright (C) 2014 Google Inc.
 * Copyright (C) 2015 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <chip.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <arch/io.h>
#include <arch/ioapic.h>
#include <arch/acpi.h>
#include <cpu/cpu.h>
#include <pc80/mc146818rtc.h>
#include <reg_script.h>
#include <string.h>
#include <soc/iomap.h>
#include <soc/pci_devs.h>
#include <soc/pmc.h>
#include <soc/pm.h>
#include <cpu/x86/smm.h>
#include <soc/pcr.h>
#include <soc/ramstage.h>

#if IS_ENABLED(CONFIG_CHROMEOS)
#include <vendorcode/google/chromeos/chromeos.h>
#endif

static const struct reg_script pch_pmc_misc_init_script[] = {
	/* Setup SLP signal assertion, SLP_S4=4s, SLP_S3=50ms */
	REG_PCI_RMW16(GEN_PMCON_B, ~((3 << 4)|(1 << 10)),
			(1 << 3)|(1 << 11)|(1 << 12)),
	REG_IO_RMW32(ACPI_BASE_ADDRESS + PM1_CNT, ~SLP_TYP, SCI_EN),
	/* Indicate DRAM init done for MRC */
	REG_PCI_OR8(GEN_PMCON_A, (1 << 23)),
	REG_SCRIPT_END
};

static void pch_pmc_add_mmio_resources(device_t dev)
{
	struct resource *res;
	const u32 default_decode_base = PCH_PCR_BASE_ADDRESS;

	/*
	 * Till PCI enumeration happens we need to allocate the PMC base
	 * statically. Above the PCR base.
	 */
	if (PCH_PWRM_BASE_ADDRESS < default_decode_base) {
		res = new_resource(dev, PWRMBASE);
		res->base = PCH_PWRM_BASE_ADDRESS;
		/* 64KB PMC size */
		res->size = 0x10000;
		res->flags = IORESOURCE_MEM | IORESOURCE_ASSIGNED |
				IORESOURCE_FIXED | IORESOURCE_RESERVE;
	}
}

static void pch_pmc_add_io_resource(device_t dev, u16 base, u16 size, int index)
{
	struct resource *res;
	res = new_resource(dev, index);
	res->base = base;
	res->size = size;
	res->flags = IORESOURCE_IO | IORESOURCE_ASSIGNED | IORESOURCE_FIXED;
}

static void pch_pmc_add_io_resources(device_t dev)
{
	/* PMBASE */
	pch_pmc_add_io_resource(dev, ACPI_BASE_ADDRESS, ACPI_BASE_SIZE, ABASE);
}

static void pch_pmc_read_resources(device_t dev)
{
	/* Get the normal PCI resources of this device. */
	pci_dev_read_resources(dev);

	/* Add non-standard MMIO resources. */
	pch_pmc_add_mmio_resources(dev);

	/* Add IO resources. */
	pch_pmc_add_io_resources(dev);
}

static void pch_set_acpi_mode(void)
{
	if (IS_ENABLED(CONFIG_HAVE_SMI_HANDLER) && acpi_slp_type != 3) {
		printk(BIOS_DEBUG, "Disabling ACPI via APMC:\n");
		outb(APM_CNT_ACPI_DISABLE, APM_CNT);
		printk(BIOS_DEBUG, "done.\n");
	}
}

#if IS_ENABLED(CONFIG_CHROMEOS_VBNV_CMOS)
/*
 * Preserve Vboot NV data when clearing CMOS as it will
 * have been re-initialized already by Vboot firmware init.
 */
static void pch_cmos_init_preserve(int reset)
{
	uint8_t vbnv[CONFIG_VBNV_SIZE];
	if (reset)
		read_vbnv(vbnv);

	cmos_init(reset);

	if (reset)
		save_vbnv(vbnv);
}
#endif

static void pch_rtc_init(void)
{
	u8 reg8;
	int rtc_failed;
	/*PMC Controller Device 0x1F, Func 02*/
	device_t dev = PCH_DEV_PMC;
	reg8 = pci_read_config8(dev, GEN_PMCON_B);
	rtc_failed = reg8 & RTC_BATTERY_DEAD;
	if (rtc_failed) {
		reg8 &= ~RTC_BATTERY_DEAD;
		pci_write_config8(dev, GEN_PMCON_B, reg8);
		printk(BIOS_DEBUG, "rtc_failed = 0x%x\n", rtc_failed);
	}

#if IS_ENABLED(CONFIG_CHROMEOS_VBNV_CMOS)
	pch_cmos_init_preserve(rtc_failed);
#else
	cmos_init(rtc_failed);
#endif
}

static void pch_power_options(void)
{
	u16 reg16;
	const char *state;
	/*PMC Controller Device 0x1F, Func 02*/
	device_t dev = PCH_DEV_PMC;
	/* Get the chip configuration */
	config_t *config = dev->chip_info;
	int pwr_on = CONFIG_MAINBOARD_POWER_ON_AFTER_POWER_FAIL;

	/*
	 * Which state do we want to goto after g3 (power restored)?
	 * 0 == S0 Full On
	 * 1 == S5 Soft Off
	 *
	 * If the option is not existent (Laptops), use Kconfig setting.
	 */
	/*TODO: cmos_layout.bin need to verify; cause wrong CMOS setup*/
	//get_option(&pwr_on, "power_on_after_fail");
	pwr_on = MAINBOARD_POWER_ON;
	reg16 = pci_read_config16(dev, GEN_PMCON_B);
	reg16 &= 0xfffe;
	switch (pwr_on) {
	case MAINBOARD_POWER_OFF:
		reg16 |= 1;
		state = "off";
		break;
	case MAINBOARD_POWER_ON:
		reg16 &= ~1;
		state = "on";
		break;
	case MAINBOARD_POWER_KEEP:
		reg16 &= ~1;
		state = "state keep";
		break;
	default:
		state = "undefined";
	}
	pci_write_config16(dev, GEN_PMCON_B, reg16);
	printk(BIOS_INFO, "Set power %s after power failure.\n", state);
	/* GPE setup based on device tree configuration */
	enable_all_gpe(config->gpe0_en_1, config->gpe0_en_2,
			config->gpe0_en_3, config->gpe0_en_4);

	/* SMI setup based on device tree configuration */
	enable_alt_smi(config->ec_smi_gpio, config->alt_gp_smi_en);
}

static void config_deep_sX(uint32_t offset, uint32_t mask, int sx, int enable)
{
	uint32_t reg;
	uint8_t *pmcbase = pmc_mmio_regs();

	printk(BIOS_DEBUG, "%sabling Deep S%c\n",
		enable ? "En" : "Dis", sx + '0');
	reg = read32(pmcbase + offset);
	if (enable)
		reg |= mask;
	else
		reg &= ~mask;
	write32(pmcbase + offset, reg);
}

static void config_deep_s5(int on)
{
	/* Treat S4 the same as S5. */
	config_deep_sX(S4_PWRGATE_POL, S4DC_GATE_SUS | S4AC_GATE_SUS, 4, on);
	config_deep_sX(S5_PWRGATE_POL, S5DC_GATE_SUS | S5AC_GATE_SUS, 5, on);
}

static void config_deep_s3(int on)
{
	config_deep_sX(S3_PWRGATE_POL, S3DC_GATE_SUS | S3AC_GATE_SUS, 3, on);
}

static void pmc_init(struct device *dev)
{
	config_t *config = dev->chip_info;

	pch_rtc_init();

	/* Initialize power management */
	pch_power_options();

	reg_script_run_on_dev(dev, pch_pmc_misc_init_script);
	pch_set_acpi_mode();

	config_deep_s3(config->deep_s3_enable);
	config_deep_s5(config->deep_s5_enable);
}

static struct device_operations device_ops = {
	.read_resources		= &pch_pmc_read_resources,
	.set_resources		= &pci_dev_set_resources,
	.enable_resources	= &pci_dev_enable_resources,
	.init			= &pmc_init,
	.scan_bus		= &scan_static_bus,
	.ops_pci		= &soc_pci_ops,
};

static const unsigned short pci_device_ids[] = {
	0x9d21,
	0
};

static const struct pci_driver pch_lpc __pci_driver = {
	.ops	 = &device_ops,
	.vendor	 = PCI_VENDOR_ID_INTEL,
	.devices = pci_device_ids,
};
