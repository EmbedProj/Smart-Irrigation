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
}

button_status get_button_status(){
	if (gpio_get_level(USER_BUTTON) == 0)
		b_stat.button_press = BUTTON_ON;
	else
		b_stat.button_press = BUTTON_OFF;
	return b_stat;
}

void user_button_dsbl(){
	gpio_reset_pin(USER_BUTTON);
	b_stat.button_en = BUTTON_DISABLE;
}

button_status user_button_test() {

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
