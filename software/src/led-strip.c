/* led-strip-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
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

void spibb_write_byte(const uint8_t value) {
	for(int8_t i = 7; i >= 0; i--) {
		if((value >> i) & 1) {
			PIN_SPI_SDI.pio->PIO_CODR = PIN_SPI_SDI.mask;
		} else {
			PIN_SPI_SDI.pio->PIO_SODR = PIN_SPI_SDI.mask;
		}

		//SLEEP_US(1);
		SLEEP_NS(250);

		PIN_SPI_CKI.pio->PIO_CODR = PIN_SPI_CKI.mask;
		//SLEEP_US(1);
		SLEEP_NS(250);
		PIN_SPI_CKI.pio->PIO_SODR = PIN_SPI_CKI.mask;
	}
}

void set_rgb_values(const ComType com, const SetRGBValues *data) {
	// TODO: If index + length to big -> error

	BC->frame_length = MAX(BC->frame_length, data->index + data->length);

	const uint8_t length = MIN(data->length, RGB_VALUE_SIZE);
	for(uint8_t i = 0; i < length; i++) {
		BC->r[data->index + i] = data->r[i];
		BC->g[data->index + i] = data->g[i];
		BC->b[data->index + i] = data->b[i];
	}

	BA->com_return_setter(com, data);
}

void get_rgb_values(const ComType com, const GetRGBValues *data) {
	// TODO: If index + length to big -> error

	GetRGBValuesReturn grgbvr;
	grgbvr.header         = data->header;
	grgbvr.header.length  = sizeof(GetRGBValuesReturn);
	for(uint16_t i = data->index; i < MAX(RGB_LENGTH, data->index + data->length); i++) {
		grgbvr.r[i-data->index] = BC->r[i];
		grgbvr.g[i-data->index] = BC->g[i];
		grgbvr.b[i-data->index] = BC->b[i];
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

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");
	adc_channel_enable(BS->adc_channel);

	PIN_SPI_SS.type = PIO_OUTPUT_1;
	PIN_SPI_SS.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_SPI_SS, 1);

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
	BC->frame_rendered = false;
	BC->frame_length = 0;
}

void destructor(void) {
	PIN_SPI_CKI.type = PIO_INPUT;
	PIN_SPI_CKI.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_SPI_CKI, 1);

	PIN_SPI_SDI.type = PIO_INPUT;
	PIN_SPI_SDI.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_SPI_SDI, 1);

	PIN_LED_SUPPLY.type = PIO_INPUT;
	PIN_LED_SUPPLY.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_LED_SUPPLY, 1);

	adc_channel_disable(BS->adc_channel);
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->frame_set_counter > 0) {
			BC->frame_set_counter--;
			if(BC->frame_set_counter == 0) {
				if(BC->frame_length > 0) {
					BC->frame_rendered = true;
				}
				BC->frame_length_callback = BC->frame_length;
			}
			return;
		}

		BC->frame_counter++;
		if(BC->frame_counter >= BC->frame_duration-2) { // TODO: Calculate time to write data!
			BC->frame_counter = 0;
			BC->frame_set_counter = 2;
			if(BC->frame_length > 0) {
				__disable_irq();
				PIN_SPI_SS.pio->PIO_CODR = PIN_SPI_SS.mask;
				for(uint8_t i = 0; i < BC->frame_length; i++) {
					spibb_write_byte(BC->b[i]);
					spibb_write_byte(BC->g[i]);
					spibb_write_byte(BC->r[i]);
				}
				PIN_SPI_SS.pio->PIO_SODR = PIN_SPI_SS.mask;
				__enable_irq();
			}
		}

	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->frame_rendered) {
			FrameRendered fr;
			BA->com_make_default_header(&fr, BS->uid, sizeof(FrameRendered), FID_FRAME_RENDERED);
			fr.length = BC->frame_length_callback;

			BA->send_blocking_with_timeout(&fr,
										   sizeof(FrameRendered),
										   *BA->com_current);

			BC->frame_rendered = false;
		}
	}
}
