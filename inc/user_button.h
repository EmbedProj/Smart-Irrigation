/*
 * button_state.h
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#ifndef INC_DRIVERS_USER_BUTTON_H_
#define INC_DRIVERS_USER_BUTTON_H_

#include "peripheral_list.h"
#include "driver/gpio.h"

typedef enum {
	BUTTON_ON = PERIPHERAL_ON,
	BUTTON_OFF = PERIPHERAL_OFF,
	BUTTON_ENABLE = PERIPHERAL_ENABLE,
	BUTTON_DISABLE = PERIPHERAL_DISABLE,
	BUTTON_PORT_WORKING = PERIPHERAL_WORKING,
	BUTTON_PORT_N_WORKING = PERIPHERAL_NOT_WORKING,
} button_status_types;

typedef struct {
	button_status_types button_press;
	button_status_types button_en;
	button_status_types button_work;
} button_status;

void user_button_enbl();

button_status get_button_status();

void user_button_dsbl();

button_status user_button_test();


#endif /* INC_DRIVERS_USER_BUTTON_H_ */
