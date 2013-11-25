/*
 * This file is part of the libpayload project.
 *
 * Copyright 2013 Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <console/console.h>
#include <arch/exception.h>
#include <stdint.h>

void exception_test(void);

static int test_abort;

void exception_undefined_instruction(uint32_t *);
void exception_software_interrupt(uint32_t *);
void exception_prefetch_abort(uint32_t *);
void exception_data_abort(uint32_t *);
void exception_not_used(uint32_t *);
void exception_irq(uint32_t *);
void exception_fiq(uint32_t *);

static void print_regs(uint32_t *regs)
{
	int i;
	// XXX
	for (i = 0; i < 16; i++) {
		if (i == 15)
			printk(BIOS_ERR, "PC");
		else if (i == 14)
			continue; /* LR */
		else if (i == 13)
			continue; /* SP */
		else if (i == 12)
			printk(BIOS_ERR, "IP");
		else
			printk(BIOS_ERR, "R%d", i);
		printk(BIOS_ERR, " = 0x%08x\n", regs[i]);
	}
}

void exception_undefined_instruction(uint32_t *regs)
{
	printk(BIOS_ERR, "exception _undefined_instruction\n");
	print_regs(regs);
	die("exception");
}

void exception_software_interrupt(uint32_t *regs)
{
	printk(BIOS_ERR, "exception _software_interrupt\n");
	print_regs(regs);
	die("exception");
}

void exception_prefetch_abort(uint32_t *regs)
{
	printk(BIOS_ERR, "exception _prefetch_abort\n");
	print_regs(regs);
	die("exception");
}

void exception_data_abort(uint32_t *regs)
{
	if (test_abort) {
		regs[15] = regs[0];
		return;
	} else {
		printk(BIOS_ERR, "exception _data_abort\n");
		print_regs(regs);
	}
	die("exception");
}

void exception_not_used(uint32_t *regs)
{
	printk(BIOS_ERR, "exception _not_used\n");
	print_regs(regs);
	die("exception");
}

void exception_irq(uint32_t *regs)
{
}

void exception_fiq(uint32_t *regs)
{
}

static inline uint32_t get_sctlr(void)
{
	return 0;
}

static inline void set_sctlr(uint32_t val)
{
}

void exception_init(void)
{
	test_abort = 1;
	printk(BIOS_ERR, "Testing exceptions\n");
	//exception_test();
	//test_abort = 0;
	printk(BIOS_ERR, "Testing exceptions: DONE\n");

}
