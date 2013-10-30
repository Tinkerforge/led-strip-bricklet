/* led-strip-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: LED Strip Bricklet specific configuration
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "led-strip.h"

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 0

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 0
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 231

#define LOGGING_LEVEL LOGGING_DEBUG
#define DEBUG_BRICKLET 0

#define BOARD_MCK 64000000

#define PIN_LED_SUPPLY (BS->pin1_ad)
#define PIN_SPI_CKI    (BS->pin2_da)
#define PIN_SPI_SDI    (BS->pin3_pwm)
#define PIN_SPI_SS     (BS->pin4_io)

#define BRICKLET_NO_OFFSET
#define INVOCATION_IN_BRICKLET_CODE

#define MAX_BRICKLET_CONFIGS

typedef struct {
	RGB rgb;

	uint16_t frame_duration;
	uint16_t frame_counter;
	uint16_t frame_set_counter;
	uint16_t frame_length;
	uint16_t frame_length_callback;
	uint16_t frame_max_length;
	int8_t rgb_bc_diff;
	uint8_t rgb_length;
	uint8_t bcs;
	bool frame_rendered;
} BrickContext;

#endif