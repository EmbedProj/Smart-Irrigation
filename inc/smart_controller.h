/*
 * smart_io_handler.h
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#ifndef INC_SMART_CONTROLLER_H_
#define INC_SMART_CONTROLLER_H_

#include "user_button.h"
#include "pump.h"

typedef enum {
	MANUAL_MODE = BUTTON_ON,
	AUTO_MODE = BUTTON_OFF,
} operation_mode;

typedef enum {
	ALL_PERIPHERALS_WORKING = PERIPHERAL_WORKING,
	USER_BUTTON_NOT_WORKING = PERIPHERAL_NOT_WORKING,
	PUMP_NOT_WORKING = PERIPHERAL_NOT_WORKING,
	ALL_PERIPHERAL_NOT_WORKING = PERIPHERAL_NOT_WORKING,
} smart_cntrl_test_res;

operation_mode get_operation_mode();

void smart_control_start();

void smart_control_stop();

void smart_control(double moisture_reading, short int user_command);

smart_cntrl_test_res smart_control_test();

#endif /* INC_HANDLERS_SMART_IO_HANDLER_H_ */
