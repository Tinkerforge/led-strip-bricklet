/* led-strip-bricklet
 * Copyright (C) 2013-2014 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led-strip.h: Implementation of LED Strip Bricklet messages
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

#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <stdint.h>
#include "bricklib/com/com_common.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define RGB_LENGTH 80
#define RGBW_LENGTH 60
#define RGB_VALUE_SIZE 16
#define RGBW_VALUE_SIZE 12

#define TYPE_WS2801  0
#define TYPE_WS2811  1
#define TYPE_WS2812  2
#define TYPE_LPD8806 3
#define TYPE_APA102  4

#define OPTION_TYPE_MASK      (7 << 1)

#define OPTION_FRAME_RENDERED (1 << 0)
#define OPTION_TYPE_WS2801    (TYPE_WS2801 << 1)
#define OPTION_TYPE_WS2811    (TYPE_WS2811 << 1)
#define OPTION_TYPE_WS2812    (TYPE_WS2812 << 1)
#define OPTION_TYPE_LPD8806   (TYPE_LPD8806 << 1)
#define OPTION_TYPE_APA102    (TYPE_APA102 << 1)
#define OPTION_DATA_CHANGED   (1 << 4)
#define OPTION_DATA_ONE_MORE  (1 << 5)
#define OPTION_4_CHANNELS     (1 << 6)

#define FID_SET_RGB_VALUES      1
#define FID_GET_RGB_VALUES      2
#define FID_SET_FRAME_DURATION  3
#define FID_GET_FRAME_DURATION  4
#define FID_GET_SUPPLY_VOLTAGE  5
#define FID_FRAME_RENDERED      6
#define FID_SET_CLOCK_FREQUENCY 7
#define FID_GET_CLOCK_FREQUENCY 8
#define FID_SET_CHIP_TYPE       9
#define FID_GET_CHIP_TYPE       10
#define FID_SET_RGBW_VALUES     11
#define FID_GET_RGBW_VALUES     12

typedef struct {
	uint8_t r[RGB_LENGTH];
	uint8_t g[RGB_LENGTH];
	uint8_t b[RGB_LENGTH];
} __attribute__((__packed__)) RGB;

typedef struct {
	uint8_t r[RGBW_LENGTH];
	uint8_t g[RGBW_LENGTH];
	uint8_t b[RGBW_LENGTH];
	uint8_t w[RGBW_LENGTH];
} __attribute__((__packed__)) RGBW;


typedef struct {
	MessageHeader header;
	uint16_t index;
	uint8_t length;
	uint8_t r[RGB_VALUE_SIZE];
	uint8_t g[RGB_VALUE_SIZE];
	uint8_t b[RGB_VALUE_SIZE];
} __attribute__((__packed__)) SetRGBValues;

typedef struct {
	MessageHeader header;
	uint16_t index;
	uint8_t length;
} __attribute__((__packed__)) GetRGBValues;

typedef struct {
	MessageHeader header;
	uint8_t r[RGB_VALUE_SIZE];
	uint8_t g[RGB_VALUE_SIZE];
	uint8_t b[RGB_VALUE_SIZE];
} __attribute__((__packed__)) GetRGBValuesReturn;

typedef struct {
	MessageHeader header;
	uint16_t duration;
} __attribute__((__packed__)) SetFrameDuration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetFrameDuration;

typedef struct {
	MessageHeader header;
	uint16_t duration;
} __attribute__((__packed__)) GetFrameDurationReturn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetSupplyVoltage;

typedef struct {
	MessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) GetSupplyVoltageReturn;

typedef struct {
	MessageHeader header;
	uint16_t length;
} __attribute__((__packed__)) FrameRendered;

typedef struct {
	MessageHeader header;
	uint32_t frequency;
} __attribute__((__packed__)) SetClockFrequency;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetClockFrequency;

typedef struct {
	MessageHeader header;
	uint32_t frequency;
} __attribute__((__packed__)) GetClockFrequencyReturn;

typedef struct {
	MessageHeader header;
	uint16_t chip;
} __attribute__((__packed__)) SetChipType;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetChipType;

typedef struct {
	MessageHeader header;
	uint16_t chip;
} __attribute__((__packed__)) GetChipTypeReturn;

typedef struct {
	MessageHeader header;
	uint16_t index;
	uint8_t length;
	uint8_t r[RGBW_VALUE_SIZE];
	uint8_t g[RGBW_VALUE_SIZE];
	uint8_t b[RGBW_VALUE_SIZE];
	uint8_t w[RGBW_VALUE_SIZE];
} __attribute__((__packed__)) SetRGBWValues;

typedef struct {
	MessageHeader header;
	uint16_t index;
	uint8_t length;
} __attribute__((__packed__)) GetRGBWValues;

typedef struct {
	MessageHeader header;
	uint8_t r[RGBW_VALUE_SIZE];
	uint8_t g[RGBW_VALUE_SIZE];
	uint8_t b[RGBW_VALUE_SIZE];
	uint8_t w[RGBW_VALUE_SIZE];
} __attribute__((__packed__)) GetRGBWValuesReturn;

void set_rgb_values(const ComType com, const SetRGBValues *data);
void get_rgb_values(const ComType com, const GetRGBValues *data);
void set_frame_duration(const ComType com, const SetFrameDuration *data);
void get_frame_duration(const ComType com, const GetFrameDuration *data);
void get_supply_voltage(const ComType com, const GetSupplyVoltage *data);
void set_clock_frequency(const ComType com, const SetClockFrequency *data);
void get_clock_frequency(const ComType com, const GetClockFrequency *data);
void set_chip_type(const ComType com, const SetChipType *data);
void get_chip_type(const ComType com, const GetChipType *data);
void set_rgbw_values(const ComType com, const SetRGBWValues *data);
void get_rgbw_values(const ComType com, const GetRGBWValues *data);

void bb_write_3byte_ws2801(const uint32_t value);
void bb_write_3byte_ws281x(const uint32_t value);
void bb_write_4byte_ws281x(const uint32_t value);
void bb_write_4byte_apa102(const uint32_t value);
void bb_write_1byte(const uint32_t value);

void option_ws2801(void);
void option_ws281x(void);
void option_lpd8806(void);
void option_apa102(void);

void set_rgb_by_global_index(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
void get_rgb_from_global_index(uint16_t index, uint8_t *r, uint8_t *g, uint8_t *b);

void set_rgbw_by_global_index(uint16_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void get_rgbw_from_global_index(uint16_t index, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif
