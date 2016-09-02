/* led-strip-bricklet
 * Copyright (C) 2013-2014 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led-strip.c: Implementation of LED Strip Bricklet messages
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

#include "led-strip.h"

#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/drivers/adc/adc.h"
#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "config.h"

#define NOP10() do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP9()  do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP8()  do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP7()  do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP6()  do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP5()  do{ __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP4()  do{ __NOP(); __NOP(); __NOP(); __NOP(); }while(0)
#define NOP3()  do{ __NOP(); __NOP(); __NOP(); }while(0)
#define NOP2()  do{ __NOP(); __NOP(); }while(0)
#define NOP1()  do{ __NOP(); }while(0)

// "PUSH {R0}\n" -> 2 cycles
// "MOV R0, %0\n" -> 1 cycles
// "SUBS R0, #1\n" -> 1 cycles
// "BNE.N 1b\n" -> 2 cycles if taken, 1 otherwise
// "POP {R0}\n" -> 2 cycles
#define SLEEP_THREE_CYCLES(x) \
	do { \
		__ASM volatile ( \
			"PUSH {R0}\n" \
			"MOV R0, %0\n" \
			"1:\n" \
			"SUBS R0, #1\n" \
			"BNE.N 1b\n" \
			"POP {R0}\n" \
			:: "r" (x) \
		); \
	} while(0)

/*   --- Cycle number is calculated with the following Python script ---
 *       !!! This is currently not used anymore !!!
CYCLE_NS = 15.63
CYCLE_HEAD = 3
CYCLE_LOOP = 3
CYCLE_LOOP_END = 2
CYCLE_TAIL = 2
CYCLE_IF = 3

def ns_to_cycle(n):
    cycles = n/15.63
    loop = cycles/3.0
    value = loop - (CYCLE_HEAD + CYCLE_TAIL + CYCLE_IF - (CYCLE_LOOP - CYCLE_LOOP_END))/3.0
    print n, value

ns_to_cycle(350)
ns_to_cycle(500)
ns_to_cycle(600)
ns_to_cycle(700)
ns_to_cycle(800)
ns_to_cycle(900)
ns_to_cycle(1200)
ns_to_cycle(1300)
ns_to_cycle(2000)
  ------------------------------------------------------------------------  */

// We sleep about 250us low phase and 1250us high phase (found by trial and error)
// This fits nearly exactly in the middle of the margins found by
// Tim: http://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/
// It does not correspond to the datasheet!

void bb_write_ws281x(const uint32_t value, const int8_t bit_count) {
	for(int8_t i = bit_count - 1; i >= 0; i--) {
		if((value >> i) & 1) {
			PIN_SPI_SDI.pio->PIO_CODR = PIN_SPI_SDI.mask;
			SLEEP_THREE_CYCLES(23);
			PIN_SPI_SDI.pio->PIO_SODR = PIN_SPI_SDI.mask;
		} else {
			PIN_SPI_SDI.pio->PIO_CODR = PIN_SPI_SDI.mask;
			SLEEP_THREE_CYCLES(3);
			PIN_SPI_SDI.pio->PIO_SODR = PIN_SPI_SDI.mask;
			SLEEP_THREE_CYCLES(17);
		}
	}
}

void bb_write_with_clock(const uint32_t value, const int8_t bit_count) {
	for(int8_t i = bit_count - 1; i >= 0; i--) {
		if((value >> i) & 1) {
			PIN_SPI_SDI.pio->PIO_CODR = PIN_SPI_SDI.mask;
		} else {
			PIN_SPI_SDI.pio->PIO_SODR = PIN_SPI_SDI.mask;
		}
		SLEEP_NS(BC->clock_delay);
		PIN_SPI_CKI.pio->PIO_CODR = PIN_SPI_CKI.mask;
		SLEEP_NS(BC->clock_delay);
		PIN_SPI_CKI.pio->PIO_SODR = PIN_SPI_CKI.mask;
	}
}

BrickContext *get_bc_from_global_index(uint16_t *index, uint8_t block_length) {
	uint8_t bc_num = 0;

	for (; bc_num < 4; bc_num++) {
		if (BC->bcs & (1 << bc_num)) {
			if (*index >= block_length) {
				*index -= block_length;
			} else {
				break;
			}
		}
	}

	if (*index > block_length) {
		return NULL;
	}

	int8_t bc_diff = -(BS->port - 'a');

	return BCO_DIRECT(bc_num + bc_diff);
}

void set_c3_by_global_index(uint16_t index, uint8_t *c3) {
	BrickContext *bc = get_bc_from_global_index(&index, BUFFER_LENGTH / 3);

	bc->c3[index][0] = c3[0];
	bc->c3[index][1] = c3[1];
	bc->c3[index][2] = c3[2];

	BC->options |= OPTION_DATA_CHANGED;
	BC->options |= OPTION_DATA_ONE_MORE;
}

void get_c3_from_global_index(uint16_t index, uint8_t *c3) {
	BrickContext *bc = get_bc_from_global_index(&index, BUFFER_LENGTH / 3);

	c3[0] = bc->c3[index][0];
	c3[1] = bc->c3[index][1];
	c3[2] = bc->c3[index][2];
}

static const uint8_t rgb_mapping[][3] = {
	/* RGB */ {0, 1, 2},
	/* RBG */ {0, 2, 1},
	/* BRG */ {2, 0, 1},
	/* BGR */ {2, 1, 0},
	/* GRB */ {1, 0, 2},
	/* GBR */ {1, 2, 0},
};

uint8_t get_rgb_mapping_index(void) {
	switch (BC->channel_mapping) {
	default:
	case CHANNEL_MAPPING_RGB:
	case CHANNEL_MAPPING_RGBW:
	case CHANNEL_MAPPING_RGWB:
	case CHANNEL_MAPPING_RWGB:
	case CHANNEL_MAPPING_WRGB: return 0;

	case CHANNEL_MAPPING_RBG:
	case CHANNEL_MAPPING_RBGW:
	case CHANNEL_MAPPING_RBWG:
	case CHANNEL_MAPPING_RWBG:
	case CHANNEL_MAPPING_WRBG: return 1;

	case CHANNEL_MAPPING_BRG:
	case CHANNEL_MAPPING_BRGW:
	case CHANNEL_MAPPING_BRWG:
	case CHANNEL_MAPPING_BWRG:
	case CHANNEL_MAPPING_WBRG: return 2;

	case CHANNEL_MAPPING_BGR:
	case CHANNEL_MAPPING_BGRW:
	case CHANNEL_MAPPING_BGWR:
	case CHANNEL_MAPPING_BWGR:
	case CHANNEL_MAPPING_WBGR: return 3;

	case CHANNEL_MAPPING_GRB:
	case CHANNEL_MAPPING_GRBW:
	case CHANNEL_MAPPING_GRWB:
	case CHANNEL_MAPPING_GWRB:
	case CHANNEL_MAPPING_WGRB: return 4;

	case CHANNEL_MAPPING_GBR:
	case CHANNEL_MAPPING_GBRW:
	case CHANNEL_MAPPING_GBWR:
	case CHANNEL_MAPPING_GWBR:
	case CHANNEL_MAPPING_WGBR: return 5;
	}
}

void set_rgb_values(const ComType com, const SetRGBValues *data) {
	if((data->index + data->length > BC->max_buffer_length / 3) || (data->length > C3_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->frame_length = MAX(BC->frame_length, data->index + data->length);
	BC->options &= ~OPTION_4_CHANNELS;

	const uint8_t *m = rgb_mapping[get_rgb_mapping_index()];

	for(uint8_t i = 0; i < data->length; i++) {
		uint8_t in[3] = {data->r[i], data->g[i], data->b[i]};
		uint8_t c3[3] = {in[m[0]], in[m[1]], in[m[2]]};
		set_c3_by_global_index(data->index + i, c3);
	}

	BA->com_return_setter(com, data);
}

void get_rgb_values(const ComType com, const GetRGBValues *data) {
	if((data->index + data->length > BC->max_buffer_length / 3) || (data->length > C3_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetRGBValuesReturn grgbvr;
	grgbvr.header         = data->header;
	grgbvr.header.length  = sizeof(GetRGBValuesReturn);

	const uint8_t *m = rgb_mapping[get_rgb_mapping_index()];
	uint8_t c3[3];

	for(uint16_t i = 0; i < data->length; i++) {
		get_c3_from_global_index(data->index + i, c3);

		grgbvr.r[i] = c3[m[0]];
		grgbvr.g[i] = c3[m[1]];
		grgbvr.b[i] = c3[m[2]];
	}

	BA->send_blocking_with_timeout(&grgbvr, sizeof(GetRGBValuesReturn), com);
}

void set_frame_duration(const ComType com, const SetFrameDuration *data) {
	BC->frame_duration = data->duration;
	BA->com_return_setter(com, data);
}

void get_frame_duration(const ComType com, const GetFrameDuration *data) {
	GetFrameDurationReturn gcr;
	gcr.header         = data->header;
	gcr.header.length  = sizeof(GetFrameDurationReturn);
	gcr.duration       = BC->frame_duration;
	BA->send_blocking_with_timeout(&gcr, sizeof(GetFrameDurationReturn), com);
}

void get_supply_voltage(const ComType com, const GetSupplyVoltage *data) {
	GetSupplyVoltageReturn gsvr;
	gsvr.header         = data->header;
	gsvr.header.length  = sizeof(GetSupplyVoltageReturn);
	// voltage divider: 10k / 1k
	gsvr.voltage        = BA->adc_channel_get_data(BS->adc_channel)*3300*11/4095;
	BA->send_blocking_with_timeout(&gsvr, sizeof(GetSupplyVoltageReturn), com);
}

void set_clock_frequency(const ComType com, const SetClockFrequency *data) {
	if(data->frequency > 2000000) {
		BC->clock_delay = 250;
	} else if (data->frequency < 10000) {
		BC->clock_delay = 50000;
	} else {
		BC->clock_delay = 1000000000/(data->frequency*2);
	}
}

void get_clock_frequency(const ComType com, const GetClockFrequency *data) {
	GetClockFrequencyReturn gcfr;
	gcfr.header         = data->header;
	gcfr.header.length  = sizeof(GetClockFrequencyReturn);
	gcfr.frequency      = 1000000000/(BC->clock_delay*2);
	BA->send_blocking_with_timeout(&gcfr, sizeof(GetClockFrequencyReturn), com);
}

void set_chip_type(const ComType com, const SetChipType *data) {
	if(data->chip != 2801 && data->chip != 2811 && data->chip != 2812 && data->chip != 8806 && data->chip != 102) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	switch(data->chip) {
		case 102:  BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_APA102;  break;
		case 2801: BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2801;  break;
		case 2811: BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2811;  break;
		case 2812: BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2812;  break;
		case 8806: BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_LPD8806; break;
		default: break;
	}

	BA->com_return_setter(com, data);
}

void get_chip_type(const ComType com, const GetChipType *data) {
	GetChipTypeReturn gctr;
	gctr.header         = data->header;
	gctr.header.length  = sizeof(GetChipTypeReturn);

	switch(BC->options & OPTION_TYPE_MASK) {
		case OPTION_TYPE_APA102:  gctr.chip = 102;  break;
		case OPTION_TYPE_WS2801:  gctr.chip = 2801; break;
		case OPTION_TYPE_WS2811:  gctr.chip = 2811; break;
		case OPTION_TYPE_WS2812:  gctr.chip = 2812; break;
		case OPTION_TYPE_LPD8806: gctr.chip = 8806; break;
		default: break;
	}

	BA->send_blocking_with_timeout(&gctr, sizeof(GetChipTypeReturn), com);
}

void set_channel_mapping(const ComType com, const SetChannelMapping *data) {
	if(data->channel_mapping > CHANNEL_MAPPING_WBRG) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->channel_mapping = data->channel_mapping;
	BA->com_return_setter(com, data);
}

void get_channel_mapping(const ComType com, const GetChannelMapping *data) {
	GetChannelMappingReturn gcmr;

	gcmr.header          = data->header;
	gcmr.header.length   = sizeof(GetChannelMappingReturn);
	gcmr.channel_mapping = BC->channel_mapping;

	BA->send_blocking_with_timeout(&gcmr, sizeof(GetChannelMappingReturn), com);
}

static const uint8_t rgbw_mapping[][4] = {
	/* RGBW */ {0, 1, 2, 3},
	/* RBGW */ {0, 2, 1, 3},
	/* BRGW */ {2, 0, 1, 3},
	/* BGRW */ {2, 1, 0, 3},
	/* GRBW */ {1, 0, 2, 3},
	/* GBRW */ {1, 2, 0, 3},

	/* RGWB */ {0, 1, 3, 2},
	/* RBWG */ {0, 2, 3, 1},
	/* RWGB */ {0, 3, 1, 2},
	/* RWBG */ {0, 3, 2, 1},
	/* GRWB */ {1, 0, 3, 2},
	/* GBWR */ {1, 2, 3, 0},
	/* GWBR */ {1, 3, 2, 0},
	/* GWRB */ {1, 3, 0, 2},
	/* BRWG */ {2, 0, 3, 1},
	/* BGWR */ {2, 1, 3, 0},
	/* BWRG */ {2, 3, 0, 1},
	/* BWGR */ {2, 3, 1, 0},
	/* WRBG */ {3, 0, 2, 1},
	/* WRGB */ {3, 0, 1, 2},
	/* WGBR */ {3, 1, 2, 0},
	/* WGRB */ {3, 1, 0, 2},
	/* WBGR */ {3, 2, 1, 0},
	/* WBRG */ {3, 2, 0, 1},
};

uint8_t get_rgbw_mapping_index(void) {
	switch (BC->channel_mapping) {
	default:
	case CHANNEL_MAPPING_RGB:
	case CHANNEL_MAPPING_RGBW: return 0;
	case CHANNEL_MAPPING_RBG:
	case CHANNEL_MAPPING_RBGW: return 1;
	case CHANNEL_MAPPING_BRG:
	case CHANNEL_MAPPING_BRGW: return 2;
	case CHANNEL_MAPPING_BGR:
	case CHANNEL_MAPPING_BGRW: return 3;
	case CHANNEL_MAPPING_GRB:
	case CHANNEL_MAPPING_GRBW: return 4;
	case CHANNEL_MAPPING_GBR:
	case CHANNEL_MAPPING_GBRW: return 5;
	case CHANNEL_MAPPING_RGWB: return 6;
	case CHANNEL_MAPPING_RBWG: return 7;
	case CHANNEL_MAPPING_RWGB: return 8;
	case CHANNEL_MAPPING_RWBG: return 9;
	case CHANNEL_MAPPING_GRWB: return 10;
	case CHANNEL_MAPPING_GBWR: return 11;
	case CHANNEL_MAPPING_GWBR: return 12;
	case CHANNEL_MAPPING_GWRB: return 13;
	case CHANNEL_MAPPING_BRWG: return 14;
	case CHANNEL_MAPPING_BGWR: return 15;
	case CHANNEL_MAPPING_BWRG: return 16;
	case CHANNEL_MAPPING_BWGR: return 18;
	case CHANNEL_MAPPING_WRBG: return 19;
	case CHANNEL_MAPPING_WRGB: return 20;
	case CHANNEL_MAPPING_WGBR: return 21;
	case CHANNEL_MAPPING_WGRB: return 22;
	case CHANNEL_MAPPING_WBGR: return 23;
	case CHANNEL_MAPPING_WBRG: return 24;
	}
}

void set_rgbw_values(const ComType com, const SetRGBWValues *data) {
	if(data->index + data->length > BC->max_buffer_length / 4 || data->length > C4_VALUE_SIZE) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->frame_length = MAX(BC->frame_length, data->index + data->length);
	BC->options |= OPTION_4_CHANNELS;

	const uint8_t *m = rgbw_mapping[get_rgbw_mapping_index()];

	for(uint8_t i = 0; i < data->length; i++) {
		const uint8_t in[4] = {data->r[i], data->g[i], data->b[i], data->w[i]};
		uint8_t c4[4] = {in[m[0]], in[m[1]], in[m[2]], in[m[3]]};
		set_c4_by_global_index(data->index + i, c4);
	}

	BA->com_return_setter(com, data);
}

void get_rgbw_values(const ComType com, const GetRGBWValues *data) {
	if(data->index + data->length > BC->max_buffer_length / 4 || data->length > C4_VALUE_SIZE) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetRGBWValuesReturn grgbwvr;
	grgbwvr.header         = data->header;
	grgbwvr.header.length  = sizeof(GetRGBWValuesReturn);

	const uint8_t *m = rgbw_mapping[get_rgbw_mapping_index()];
	uint8_t c4[4];

	for(uint16_t i = 0; i < data->length; i++) {
		get_c4_from_global_index(data->index + i, c4);

		grgbwvr.r[i] = c4[m[0]];
		grgbwvr.g[i] = c4[m[1]];
		grgbwvr.b[i] = c4[m[2]];
		grgbwvr.w[i] = c4[m[3]];
	}

	BA->send_blocking_with_timeout(&grgbwvr, sizeof(GetRGBWValuesReturn), com);
}

void get_c4_from_global_index(uint16_t index, uint8_t *c4) {
	BrickContext *bc = get_bc_from_global_index(&index, BUFFER_LENGTH / 4);

	c4[0] = bc->c4[index][0];
	c4[1] = bc->c4[index][1];
	c4[2] = bc->c4[index][2];
	c4[3] = bc->c4[index][3];
}

void set_c4_by_global_index(uint16_t index, uint8_t *c4) {
	BrickContext *bc = get_bc_from_global_index(&index, BUFFER_LENGTH / 4);

	bc->c4[index][0] = c4[0];
	bc->c4[index][1] = c4[1];
	bc->c4[index][2] = c4[2];
	bc->c4[index][3] = c4[3];

	BC->options |= OPTION_DATA_CHANGED;
	BC->options |= OPTION_DATA_ONE_MORE;
}

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_SET_RGB_VALUES: {
			set_rgb_values(com, (SetRGBValues*)data);
			return;
		}

		case FID_GET_RGB_VALUES: {
			get_rgb_values(com, (GetRGBValues*)data);
			return;
		}

		case FID_SET_FRAME_DURATION: {
			set_frame_duration(com, (SetFrameDuration*)data);
			return;
		}

		case FID_GET_FRAME_DURATION: {
			get_frame_duration(com, (GetFrameDuration*)data);
			return;
		}

		case FID_GET_SUPPLY_VOLTAGE: {
			get_supply_voltage(com, (GetSupplyVoltage*)data);
			return;
		}

		case FID_SET_CLOCK_FREQUENCY: {
			set_clock_frequency(com, (SetClockFrequency*)data);
			return;
		}

		case FID_GET_CLOCK_FREQUENCY: {
			get_clock_frequency(com, (GetClockFrequency*)data);
			return;
		}

		case FID_SET_CHIP_TYPE: {
			set_chip_type(com, (SetChipType*)data);
			return;
		}

		case FID_GET_CHIP_TYPE: {
			get_chip_type(com, (GetChipType*)data);
			return;
		}

		case FID_SET_RGBW_VALUES: {
			set_rgbw_values(com, (SetRGBWValues*)data);
			return;
		}

		case FID_GET_RGBW_VALUES: {
			get_rgbw_values(com, (GetRGBWValues*)data);
			return;
		}

		case FID_SET_CHANNEL_MAPPING: {
			set_channel_mapping(com, (SetChannelMapping*)data);
			return;
		}

		case FID_GET_CHANNEL_MAPPING: {
			get_channel_mapping(com, (GetChannelMapping*)data);
			return;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");
	adc_channel_enable(BS->adc_channel);

	PIN_SPI_CKI.type = PIO_OUTPUT_0;
	PIN_SPI_CKI.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_SPI_CKI, 1);

	PIN_SPI_SDI.type = PIO_OUTPUT_0;
	PIN_SPI_SDI.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_SPI_SDI, 1);

	PIN_LED_SUPPLY.type = PIO_INPUT;
	PIN_LED_SUPPLY.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_LED_SUPPLY, 1);
	PIN_LED_SUPPLY.attribute = PIO_PULLDOWN;
	BA->PIO_Configure(&PIN_LED_SUPPLY, 1);

	BC->frame_duration = 100;
	BC->frame_counter = 0;
	BC->frame_set_counter = 0;
	BC->frame_length = 0;
	BC->clock_delay = 300;
	BC->channel_mapping = CHANNEL_MAPPING_BGR;

	BC->options = 0; // Frame rendered = false, Chip Type = WS2801
	BC->max_buffer_length = BUFFER_LENGTH;
	BC->bcs = 0;
}

void option_ws2801(void) {
	uint8_t c3[3];

	for(uint16_t i = 0; i < BC->frame_length; i++) {
		get_c3_from_global_index(i, c3);
		bb_write_with_clock((c3[0] << 16) | (c3[1] << 8) | c3[2], BYTES_3);
	}
}

void option_ws281x(void) {
	uint8_t c4[4];

	if (BC->options & OPTION_4_CHANNELS) {
		for(uint16_t i = 0; i < BC->frame_length; i++) {
			get_c4_from_global_index(i, c4);
			bb_write_ws281x((c4[0] << 24) | (c4[1] << 16) | (c4[2] << 8) | c4[3], BYTES_4);
		};
	} else {
		for(uint16_t i = 0; i < BC->frame_length; i++) {
			get_c3_from_global_index(i, c4);
			bb_write_ws281x((c4[0] << 16) | (c4[1] << 8) | c4[2], BYTES_3);
		}
	}
}

void option_lpd8806(void) {
	uint8_t c3[3];

	for(uint16_t i = 0; i < BC->frame_length; i++) {
		get_c3_from_global_index(i, c3);

		// +128 because the MSB has to be high while data shifting
		bb_write_with_clock(((c3[0] / 2 + 128) << 16) | ((c3[1] / 2 + 128) << 8) | (c3[2] / 2 + 128), BYTES_3);
	}

	// When MSB is low the shift registers resets and are ready for new data
	bb_write_with_clock(0, BYTES_3);
}

void option_apa102(void) {
	bb_write_with_clock(0, BYTES_4);

	uint8_t c4[4];

	for(uint16_t i = 0; i < BC->frame_length; i++) {
		get_c4_from_global_index(i, c4);

		// 3-Bit "1" and brightness setting 5-Bit: constant current output value
		bb_write_with_clock(((0b11100000 | (c4[3] / 8)) << 24) | (c4[0] << 16) | (c4[1] << 8) | c4[2], BYTES_4);
	}

	// The datasheet says that there have to be a 4-byte endframe with all bits
	// set. But that create a fully white LED after the end of the configured
	// number of LEDs. The endframe is necessary without it the last configured
	// LED doesn't work correctly. Instead of sending the documented endframe,
	// only set the first three bit to 1. This works as well and dosn't create
	// a while LED at the end. Surprisingly it doesn't produce a black one either.
	// It just works as expected.
	bb_write_with_clock(0b11100000, BYTES_4);
}

void tick(const uint8_t tick_type) {
	BrickContext *bc = BC;

	if(bc->frame_duration == 0) {
		return;
	}

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(bc->bcs == 0) {
			int8_t bc_diff = -(BS->port - 'a');
			uint8_t length;

			if(BSO_DIRECT(bc_diff+1)->address == I2C_EEPROM_ADDRESS_HIGH) {
				length = 4;
			} else {
				length = 2;
			}

			for(uint8_t i = 0; i < length; i++) {
				if(BSO_DIRECT(i + bc_diff)->device_identifier == 0) {
					bc->bcs |= 1 << i;
					bc->max_buffer_length += BUFFER_LENGTH;
				}
			}

			bc->bcs |= 1 << ABS(bc_diff);
		}

		if(bc->frame_set_counter > 0) {
			bc->frame_set_counter--;
			if(bc->frame_set_counter == 0) {
				if(bc->frame_length > 0) {
					bc->options |= OPTION_FRAME_RENDERED;
				}
			}
			return;
		}

		bc->frame_counter++;

		if(bc->frame_counter >= bc->frame_duration-2) { // TODO: Calculate time to write data!
			bc->frame_counter = 0;
			bc->frame_set_counter = 2;

			if(bc->frame_length > 0) {
				if(bc->options & OPTION_DATA_CHANGED) {
					if(bc->options & OPTION_DATA_ONE_MORE) {
						bc->options &= ~OPTION_DATA_ONE_MORE;
					} else {
						bc->options &= ~OPTION_DATA_CHANGED;
					}

					__disable_irq();
					switch(bc->options & OPTION_TYPE_MASK) {
						case OPTION_TYPE_WS2801:  option_ws2801();  break;
						case OPTION_TYPE_WS2811:
						case OPTION_TYPE_WS2812:  option_ws281x();  break;
						case OPTION_TYPE_LPD8806: option_lpd8806(); break;
						case OPTION_TYPE_APA102:  option_apa102();  break;
					}
					__enable_irq();
				}
			}
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(bc->options & OPTION_FRAME_RENDERED) {
			FrameRendered fr;
			BA->com_make_default_header(&fr, BS->uid, sizeof(FrameRendered), FID_FRAME_RENDERED);
			fr.length = bc->frame_length;

			BA->send_blocking_with_timeout(&fr,
										   sizeof(FrameRendered),
										   *BA->com_current);

			bc->options &= ~OPTION_FRAME_RENDERED;
		}
	}
}
