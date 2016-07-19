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
void bb_write_3byte_ws2812(const uint32_t value) {
	for(int8_t i = 23; i >= 0; i--) {
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
void bb_write_4byte_ws2812(const uint32_t value) {
	for(int8_t i = 31; i >= 0; i--) {
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

void bb_write_3byte_ws2801(const uint32_t value) {
	for(int8_t i = 23; i >= 0; i--) {
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

void set_rgb_by_global_index(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t bc_num = 0;
	for(; bc_num < 4; bc_num++) {
		if(BC->bcs & (1 << bc_num)) {
			if(index >= RGB_LENGTH) {
				index -= RGB_LENGTH;
			} else {
				break;
			}
		}
	}

	if(index > RGB_LENGTH) {
		return;
	}

	BrickContext *bc = BCO_DIRECT(bc_num + BC->rgb_bc_diff);

	bc->rgb.r[index] = r;
	bc->rgb.g[index] = g;
	bc->rgb.b[index] = b;

	BC->options |= OPTION_DATA_CHANGED;
	BC->options |= OPTION_DATA_ONE_MORE;
}

void get_rgb_from_global_index(uint16_t index, uint8_t *r, uint8_t *g, uint8_t *b) {
	uint8_t bc_num = 0;
	for(; bc_num < 4; bc_num++) {
		if(BC->bcs & (1 << bc_num)) {
			if(index >= RGB_LENGTH) {
				index -= RGB_LENGTH;
			} else {
				break;
			}
		}
	}

	if(index > RGB_LENGTH) {
		return;
	}

	BrickContext *bc = BCO_DIRECT(bc_num + BC->rgb_bc_diff);

	*r = bc->rgb.r[index];
	*g = bc->rgb.g[index];
	*b = bc->rgb.b[index];
}

void set_rgb_values(const ComType com, const SetRGBValues *data) {
	if((data->index + data->length > BC->frame_max_length) || (data->length > RGB_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->frame_length = MAX(BC->frame_length, data->index + data->length);
	BC->options &= ~OPTION_4_CHANNELS;

	for(uint8_t i = 0; i < data->length; i++) {
		set_rgb_by_global_index(data->index + i, data->r[i], data->g[i], data->b[i]);
	}

	BA->com_return_setter(com, data);
}



void get_rgb_values(const ComType com, const GetRGBValues *data) {
	if((data->index + data->length > BC->frame_max_length) || (data->length > RGB_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetRGBValuesReturn grgbvr;
	grgbvr.header         = data->header;
	grgbvr.header.length  = sizeof(GetRGBValuesReturn);
	for(uint16_t i = data->index; i < data->index + data->length; i++) {
		get_rgb_from_global_index(i, &grgbvr.r[i-data->index], &grgbvr.g[i-data->index], &grgbvr.b[i-data->index]);
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
	if(data->chip != 2801 && data->chip != 2811 && data->chip != 2812) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	switch(data->chip) {
		case 2801:  BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2801;  break;
		case 2811:  BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2811;  break;
		case 2812:  BC->options = (BC->options & (~OPTION_TYPE_MASK)) | OPTION_TYPE_WS2812;  break;
		default: break;
	}

	BA->com_return_setter(com, data);
}

void get_chip_type(const ComType com, const GetChipType *data) {
	GetChipTypeReturn gctr;
	gctr.header         = data->header;
	gctr.header.length  = sizeof(GetChipTypeReturn);

	switch(BC->options & OPTION_TYPE_MASK) {
		case OPTION_TYPE_WS2801:  gctr.chip = 2801;  break;
		case OPTION_TYPE_WS2811:  gctr.chip = 2811;  break;
		case OPTION_TYPE_WS2812:  gctr.chip = 2812;  break;
		default: break;
	}

	BA->send_blocking_with_timeout(&gctr, sizeof(GetChipTypeReturn), com);
}

void set_rgbw_values(const ComType com, const SetRGBWValues *data) {
	if((data->index + data->length > BC->frame_max_length) || (data->length > RGBW_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->frame_length = MAX(BC->frame_length, data->index + data->length);
	BC->options |= OPTION_4_CHANNELS;

	for(uint8_t i = 0; i < data->length; i++) {
		set_rgbw_by_global_index(data->index + i, data->r[i], data->g[i], data->b[i], data->w[i]);
	}

	BA->com_return_setter(com, data);
}
void get_rgbw_values(const ComType com, const GetRGBWValues *data) {
	if((data->index + data->length > BC->frame_max_length) || (data->length > RGBW_VALUE_SIZE)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetRGBWValuesReturn grgbwvr;
	grgbwvr.header         = data->header;
	grgbwvr.header.length  = sizeof(GetRGBWValuesReturn);
	for(uint16_t i = data->index; i < data->index + data->length; i++) {
		get_rgbw_from_global_index(i, &grgbwvr.r[i-data->index], &grgbwvr.g[i-data->index], &grgbwvr.b[i-data->index], &grgbwvr.w[i-data->index]);
	}

	BA->send_blocking_with_timeout(&grgbwvr, sizeof(GetRGBWValuesReturn), com);
}

void get_rgbw_from_global_index(uint16_t index, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) {
	uint8_t bc_num = 0;
	for(; bc_num < 4; bc_num++) {
		if(BC->bcs & (1 << bc_num)) {
			if(index >= RGBW_LENGTH) {
				index -= RGBW_LENGTH;
			} else {
				break;
			}
		}
	}

	if(index > RGBW_LENGTH) {
		return;
	}

	BrickContext *bc = BCO_DIRECT(bc_num + BC->rgb_bc_diff);

	*r = bc->rgbw.r[index];
	*g = bc->rgbw.g[index];
	*b = bc->rgbw.b[index];
	*w = bc->rgbw.w[index];
}

void set_rgbw_by_global_index(uint16_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	uint8_t bc_num = 0;
	for(; bc_num < 4; bc_num++) {
		if(BC->bcs & (1 << bc_num)) {
			if(index >= RGBW_LENGTH) {
				index -= RGBW_LENGTH;
			} else {
				break;
			}
		}
	}

	if(index > RGBW_LENGTH) {
		return;
	}

	BrickContext *bc = BCO_DIRECT(bc_num + BC->rgb_bc_diff);

	bc->rgbw.r[index] = r;
	bc->rgbw.g[index] = g;
	bc->rgbw.b[index] = b;
	bc->rgbw.w[index] = w;

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

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void reconfigure_bcs(void) {
	BC->rgb_bc_diff = -(BS->port - 'a');

	if(BSO_DIRECT(BC->rgb_bc_diff+1)->address == I2C_EEPROM_ADDRESS_HIGH) {
		BC->rgb_length = 4;
	} else {
		BC->rgb_length = 2;
	}

	BC->frame_max_length = RGB_LENGTH;
	BC->bcs = 0;
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

	BC->options = 0; // Frame rendered = false, Chip Type = WS2801
	reconfigure_bcs();
}

void tick(const uint8_t tick_type) {
	if(BC->frame_duration == 0) {
		return;
	}

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->bcs == 0) {
			for(uint8_t i = 0; i < BC->rgb_length; i++) {
				if(BSO_DIRECT(i + BC->rgb_bc_diff)->device_identifier == 0) {
					BC->bcs |= 1 << i;
					BC->frame_max_length += RGB_LENGTH;
				}
			}
			BC->bcs |= 1 << ABS(BC->rgb_bc_diff);
		}

		if(BC->frame_set_counter > 0) {
			BC->frame_set_counter--;
			if(BC->frame_set_counter == 0) {
				if(BC->frame_length > 0) {
					BC->options |= OPTION_FRAME_RENDERED;
				}
			}
			return;
		}

		BC->frame_counter++;
		if(BC->frame_counter >= BC->frame_duration-2) { // TODO: Calculate time to write data!
			BC->frame_counter = 0;
			BC->frame_set_counter = 2;
			if(BC->frame_length > 0) {
				if(BC->options & OPTION_DATA_CHANGED) {
					if(BC->options & OPTION_DATA_ONE_MORE) {
						BC->options = BC->options & (~OPTION_DATA_ONE_MORE);
					} else {
						BC->options = BC->options & (~OPTION_DATA_CHANGED);
					}
					__disable_irq();
					switch(BC->options & OPTION_TYPE_MASK) {
						case OPTION_TYPE_WS2801: {
							for(uint16_t i = 0; i < BC->frame_length; i++) {
								uint8_t r = 0;
								uint8_t g = 0;
								uint8_t b = 0;

								get_rgb_from_global_index(i, &r, &g, &b);
								bb_write_3byte_ws2801((b << 16) | (g << 8) | r);
							}
							break;
						}

						case OPTION_TYPE_WS2811:
						case OPTION_TYPE_WS2812: {
							uint8_t r = 0;
							uint8_t g = 0;
							uint8_t b = 0;
							uint8_t w = 0;
								
							if (BC->options & OPTION_4_CHANNELS) {
								for(uint16_t i = 0; i < BC->frame_length; i++) {
									get_rgbw_from_global_index(i, &r, &g, &b, &w);
									bb_write_4byte_ws2812((b << 24) | (g << 16) | (r << 8) | w);
								};
							} else { 
								for(uint16_t i = 0; i < BC->frame_length; i++) {
									get_rgb_from_global_index(i, &r, &g, &b);
									bb_write_3byte_ws2812((b << 16) | (g << 8) | r);
								}
							}
							break;
						}
					}
					__enable_irq();
				}
			}
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->options & OPTION_FRAME_RENDERED) {
			FrameRendered fr;
			BA->com_make_default_header(&fr, BS->uid, sizeof(FrameRendered), FID_FRAME_RENDERED);
			fr.length = BC->frame_length;

			BA->send_blocking_with_timeout(&fr,
										   sizeof(FrameRendered),
										   *BA->com_current);

			BC->options = BC->options & (~OPTION_FRAME_RENDERED);
		}
	}
}
