/*
 * button_state.c
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */
#include "user_button.h"

button_status b_stat;

void user_button_enbl(){
	gpio_set_direction(USER_BUTTON, GPIO_MODE_INPUT);
	gpio_set_pull_mode(USER_BUTTON, GPIO_PULLUP_ONLY);
	b_stat.button_en = BUTTON_ENABLE;
	printf('User Button Enabled\n');
}

button_status get_button_status(){
	// Reading the button press status
	// GPIO pin connected to button is internally pulled up (pin level - 1)
	// On button press, pin is pulled down (expected pin level - 0)

	if (gpio_get_level(USER_BUTTON) == 0) {
		printf("Button Pressed\n");
		b_stat.button_press = BUTTON_ON;
	}
	else {
		printf("Button Not Pressed\n");
		b_stat.button_press = BUTTON_OFF;
	}
	return b_stat;
}

void user_button_dsbl(){
	gpio_reset_pin(USER_BUTTON);
	b_stat.button_en = BUTTON_DISABLE;
	printf("User Button Disabled\n");
}

button_status user_button_test() {
	// Testing whether the GPIO pin connected to user_button is working fine
	// Test 1: pin pulled up, read pin level (expected - 1)
	// Test 2: pin pulled down, read pin level (expected - 0)
	// Port passes test if both the tests are passed


	int test_succ_count = 0;

	if (gpio_get_level(USER_BUTTON) == 1)
		test_succ_count += 1;

	gpio_set_pull_mode(USER_BUTTON, GPIO_PULLDOWN_ONLY);
	if (gpio_get_level(USER_BUTTON) == 0)
		test_succ_count += 1;
	gpio_set_pull_mode(USER_BUTTON, GPIO_PULLUP_ONLY);

	printf("User Button - Port - passed %d out of %d tests\n", test_succ_count, 2);

	if (test_succ_count == 2) {
		b_stat.button_work = BUTTON_PORT_WORKING;
	}
	else {
		b_stat.button_work = BUTTON_PORT_N_WORKING;
	}
	return b_stat;
}
