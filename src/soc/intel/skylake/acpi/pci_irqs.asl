/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

Method(_PRT)
{
	If (PICM) {
		Return (Package() {
		/* PCI Bridge */
		/* D31: cAVS, SMBus, GbE, Nothpeak */
			Package(){0x001FFFFF, 0, 0, 16 },
			Package(){0x001FFFFF, 1, 0, 17 },
			Package(){0x001FFFFF, 2, 0, 18 },
			Package(){0x001FFFFF, 3, 0, 19 },
		/* D30: SerialIo and SCS */
			Package(){0x001EFFFF, 0, 0, 20 },
			Package(){0x001EFFFF, 1, 0, 21 },
			Package(){0x001EFFFF, 2, 0, 22 },
			Package(){0x001EFFFF, 3, 0, 23 },
		/* D29: PCI Express Port 9-16 */
			Package(){0x001DFFFF, 0, 0, 16 },
			Package(){0x001DFFFF, 1, 0, 17 },
			Package(){0x001DFFFF, 2, 0, 18 },
			Package(){0x001DFFFF, 3, 0, 19 },
		/* D28: PCI Express Port 1-8 */
			Package(){0x001CFFFF, 0, 0, 16 },
			Package(){0x001CFFFF, 1, 0, 17 },
			Package(){0x001CFFFF, 2, 0, 18 },
			Package(){0x001CFFFF, 3, 0, 19 },
		/* D27: PCI Express Port 17-20 */
			Package(){0x001BFFFF, 0, 0, 16 },
			Package(){0x001BFFFF, 1, 0, 17 },
			Package(){0x001BFFFF, 2, 0, 18 },
			Package(){0x001BFFFF, 3, 0, 19 },
		/* D25: SerialIo */
			Package(){0x0019FFFF, 0, 0, 32 },
			Package(){0x0019FFFF, 1, 0, 33 },
			Package(){0x0019FFFF, 2, 0, 34 },
		/* D22: CSME (HECI, IDE-R, Keyboard and Text redirection */
			Package(){0x0016FFFF, 0, 0, 16 },
			Package(){0x0016FFFF, 1, 0, 17 },
			Package(){0x0016FFFF, 2, 0, 18 },
			Package(){0x0016FFFF, 3, 0, 19 },
		/* D21: SerialIo */
			Package(){0x0015FFFF, 0, 0, 16 },
			Package(){0x0015FFFF, 1, 0, 17 },
			Package(){0x0015FFFF, 2, 0, 18 },
			Package(){0x0015FFFF, 3, 0, 19 },
		/* D20: xHCI, OTG,
		 * Thermal Subsystem, Camera IO Host Controller
		 */
			Package(){0x0014FFFF, 0, 0, 16 },
			Package(){0x0014FFFF, 1, 0, 17 },
			Package(){0x0014FFFF, 2, 0, 18 },
			Package(){0x0014FFFF, 3, 0, 19 },
		/* D19: Integrated Sensor Hub */
			Package(){0x0013FFFF, 0, 0, 20 },

		/* Host Bridge */
		/* P.E.G. Root Port D1F0 */
			Package(){0x0001FFFF, 0, 0, 16 },
			Package(){0x0001FFFF, 1, 0, 17 },
			Package(){0x0001FFFF, 2, 0, 18 },
			Package(){0x0001FFFF, 3, 0, 19 },
		/* P.E.G. Root Port D1F1 */
		/* P.E.G. Root Port D1F2 */
		/* SA IGFX Device */
			Package(){0x0002FFFF, 0, 0, 16 },
		/* SA Thermal Device */
			Package(){0x0004FFFF, 0, 0, 16 },
		/* SA SkyCam Device */
			Package(){0x0005FFFF, 0, 0, 16 },
		/* SA GMM Device */
			Package(){0x0008FFFF, 0, 0, 16 },
		})
	} Else {
		Return (Package() {
		/* D31 */
		Package() { 0x001fffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x001fffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x001fffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x001fffff, 3, \_SB.PCI0.LNKD, 0 },
		/* D29 */
		Package() { 0x001dffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x001dffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x001dffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x001dffff, 3, \_SB.PCI0.LNKD, 0 },
		/* D28 */
		Package() { 0x001cffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x001cffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x001cffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x001cffff, 3, \_SB.PCI0.LNKD, 0 },
		/* D27 */
		Package() { 0x001bffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x001bffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x001bffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x001bffff, 3, \_SB.PCI0.LNKD, 0 },
		/* D23 */
		Package() { 0x0017ffff, 0, \_SB.PCI0.LNKA, 0 },
		/* D22 */
		Package() { 0x0016ffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x0016ffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x0016ffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x0016ffff, 3, \_SB.PCI0.LNKD, 0 },
		/* D20 */
		Package() { 0x0014ffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x0014ffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x0014ffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x0014ffff, 3, \_SB.PCI0.LNKD, 0 },
		/* Host bridge */
		Package() { 0x0001ffff, 0, \_SB.PCI0.LNKA, 0 },
		Package() { 0x0001ffff, 1, \_SB.PCI0.LNKB, 0 },
		Package() { 0x0001ffff, 2, \_SB.PCI0.LNKC, 0 },
		Package() { 0x0001ffff, 3, \_SB.PCI0.LNKD, 0 },
		/* SA IGFX Device */
		Package() { 0x0002ffff, 0, \_SB.PCI0.LNKA, 0 },
		/* SA Thermal Device */
		Package() { 0x0004ffff, 0, \_SB.PCI0.LNKA, 0 },
		/* SA Skycam Device */
		Package() { 0x0005ffff, 0, \_SB.PCI0.LNKA, 0 },
		/* SA GMM Device */
		Package() { 0x0008ffff, 0, \_SB.PCI0.LNKA, 0 },
		})
	}
}

