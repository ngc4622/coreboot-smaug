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

#ifndef SAMUS_BOARD_VERSION_H
#define SAMUS_BOARD_VERSION_H

#define SAMUS_EC_BOARD_VERSION_EVT1	3
#define SAMUS_EC_BOARD_VERSION_EVT2	4
#define SAMUS_EC_BOARD_VERSION_EVT3	5
#define SAMUS_EC_BOARD_VERSION_EVT4	0

const char *samus_board_version(void);

#endif
