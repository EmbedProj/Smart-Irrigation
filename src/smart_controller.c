/*
 * smart_io_handler.c
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#include "smart_controller.h"

void smart_control_start() {
	printf("Enabling User button\n");
	user_button_enbl();
	printf("Initializing Pump\n");
	pump_enbl();
	printf("Initializing pump status led\n");
	status_led_enbl();
}

void smart_control_stop() {
	printf("Disabling User button\n");
	user_button_dsbl();
	printf("De-initializing Pump\n");
	pump_dsbl();
	printf("De-initializing pump status led\n");
	status_led_dsbl();
}

operation_mode get_operation_mode() {
	// If button is pressed by user, then operation shifted to MANUAL_MODE
	// Otherwise the operation is always in AUTO_MODE

	if (get_button_status().button_press == BUTTON_ON)
		return MANUAL_MODE;
	else
		return AUTO_MODE;
}

void smart_control(double moisture_reading, short int user_command) {
	// Sample smart controller code
	// Exact conditions to be updated shortly

	// If moisture reading is < 10 and the operation is in Auto mode, then start the pump
	// If operation is in manual mode and the user commanded to start the pump, then start the pump
	// In all other cases the pump should be stopped (including when moisture reading < 10,
	// but operation in manual mode & user not commanded to start the pump)

	if (moisture_reading < 10.0 && get_operation_mode() == AUTO_MODE) {
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

smart_cntrl_test_res smart_control_test() {

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
		printf("Issue with both the button and pump ports");
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
