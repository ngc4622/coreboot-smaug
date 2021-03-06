/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#if IS_ENABLED(CONFIG_PLATFORM_USES_FSP1_1)
#include <fsp_util.h>
#endif	/* CONFIG_PLATFORM_USES_FSP1_1 */
#include <soc/pci_devs.h>
#include <soc/ramstage.h>
#include <soc/intel/broadwell/chip.h>

static void pci_domain_set_resources(device_t dev)
{
	assign_resources(dev->link_list);
}

static struct device_operations pci_domain_ops = {
	.read_resources   = &pci_domain_read_resources,
	.set_resources    = &pci_domain_set_resources,
	.scan_bus         = &pci_domain_scan_bus,
	.ops_pci_bus      = &pci_ops_mmconf,
};

static void cpu_bus_noop(device_t dev) { }

static void broadwell_final(device_t dev)
{
#if IS_ENABLED(CONFIG_PLATFORM_USES_FSP1_1)
	/* Notify FSP done device setup */
	printk(BIOS_DEBUG,
		"Calling FspNotify(EnumInitPhaseAfterPciEnumeration)\n");
	fsp_notify(EnumInitPhaseAfterPciEnumeration);

	printk(BIOS_DEBUG, "Calling FspNotify(EnumInitPhaseReadyToBoot)\n");
	fsp_notify(EnumInitPhaseReadyToBoot);

	printk(BIOS_DEBUG, "FspNotify Returned\n");
#endif	/* CONFIG_PLATFORM_USES_FSP1_1 */
}

static struct device_operations cpu_bus_ops = {
	.read_resources   = &cpu_bus_noop,
	.set_resources    = &cpu_bus_noop,
	.enable_resources = &cpu_bus_noop,
	.init             = &broadwell_init_cpus,
	.final            = &broadwell_final,
};

static void broadwell_enable(device_t dev)
{
	/* Set the operations if it is a special bus type */
	if (dev->path.type == DEVICE_PATH_DOMAIN) {
		dev->ops = &pci_domain_ops;
	} else if (dev->path.type == DEVICE_PATH_CPU_CLUSTER) {
		dev->ops = &cpu_bus_ops;
	} else if (dev->path.type == DEVICE_PATH_PCI) {
		/* Handle PCH device enable */
		if (PCI_SLOT(dev->path.pci.devfn) > SA_DEV_SLOT_MINIHD &&
		    (dev->ops == NULL || dev->ops->enable == NULL)) {
			broadwell_pch_enable_dev(dev);
		}
	}
}

struct chip_operations soc_intel_broadwell_ops = {
	CHIP_NAME("Intel Broadwell")
	.enable_dev = &broadwell_enable,
	.init       = &broadwell_init_pre_device,
};

static void pci_set_subsystem(device_t dev, unsigned vendor, unsigned device)
{
	if (!vendor || !device)
		pci_write_config32(dev, PCI_SUBSYSTEM_VENDOR_ID,
				   pci_read_config32(dev, PCI_VENDOR_ID));
	else
		pci_write_config32(dev, PCI_SUBSYSTEM_VENDOR_ID,
				   (device << 16) | vendor);
}

struct pci_operations broadwell_pci_ops = {
	.set_subsystem = &pci_set_subsystem
};
