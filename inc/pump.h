/*
 * pump.h
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#ifndef INC_DRIVERS_PUMP_H_
#define INC_DRIVERS_PUMP_H_

#include "peripheral_list.h"
#include "driver/gpio.h"

typedef enum {
	PUMP_ON = PERIPHERAL_ON,
	PUMP_OFF = PERIPHERAL_OFF,
	PUMP_ENABLE = PERIPHERAL_ENABLE,
	PUMP_DISABLE = PERIPHERAL_DISABLE,
	PUMP_PORT_WORKING = PERIPHERAL_WORKING,
	PUMP_PORT_N_WORKING = PERIPHERAL_NOT_WORKING,
} pump_status_types;

typedef struct {
	pump_status_types pump_power;
	pump_status_types pump_en;
	pump_status_types pump_work;
} pump_status;

void pump_enbl();

void pump_dsbl();

void pump_start();

void pump_stop();

void status_led_enbl();

void status_led_dsbl();

void status_led_on();

void status_led_off();

pump_status get_pump_status();

pump_status pump_test();

#endif /* INC_DRIVERS_PUMP_H_ */
