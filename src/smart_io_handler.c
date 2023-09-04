/*
 * smart_io_handler.c
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#include "smart_io_handler.h"

void smart_control_start() {
	user_button_enbl();
	pump_enbl();
	status_led_enbl();
}

void smart_control_stop() {
	user_button_dsbl();
	pump_dsbl();
	status_led_dsbl();
}

operation_mode get_operation_mode() {
	operation_mode opmode;
	if (get_button_status().button_press == BUTTON_ON)
		opmode = MANUAL_MODE;
	else
		opmode = AUTO_MODE;
	return opmode;
}

void smart_control(double moisture_reading, short int user_command) {
	if (moisture_reading > 10.0 && get_operation_mode() == AUTO_MODE) {
		pump_start();
		status_led_on();
	}
	else if (get_operation_mode() == MANUAL_MODE && user_command == PUMP_ON) {
		pump_start();
		status_led_on();
	}
	else {
		pump_stop();
		status_led_off();
	}
}

smart_hndlr_test_res smart_control_test() {

	int test_score = 0;

	printf("User Button - Port - Test Initiated\n");
	button_status button_test_res = user_button_test();

	if (button_test_res.button_work == BUTTON_PORT_N_WORKING) {
		printf("User Button - Port - !!! Test Failed !!!\n");
	}
	else {
		printf("User Button - Port - Test Passed\n");
		test_score += 1;
	}

	printf("Pump - Port - Test Initialized\n");
	pump_status pump_test_res = pump_test();

	if (pump_test_res.pump_work == PUMP_PORT_N_WORKING) {
		printf("Pump - Port - !!! Test Failed !!!\n");
	}
	else {
		printf("Pump - Port - Test Passed\n");
		test_score += 2;
	}

	if (test_score == 0) {
		return ALL_PERIPHERAL_NOT_WORKING;
	}
	else if (test_score == 1) {
		return PUMP_NOT_WORKING;
	}
	else if (test_score == 2) {
		return USER_BUTTON_NOT_WORKING;
	}
	else {
		return ALL_PERIPHERALS_WORKING;
	}
}
