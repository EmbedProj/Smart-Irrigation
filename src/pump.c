/*
 * pump.c
 *
 *  Created on: 02-Sep-2023
 *      Author: Phani Kiran Maddukuri
 */

#include "pump.h"

pump_status p_stat;

void pump_enbl() {
	gpio_set_direction(PUMP_CONTROLLER,GPIO_MODE_OUTPUT);
	p_stat.pump_en = PUMP_ENABLE;
	pump_stop();
	printf("Pump Initialized\n");
}

void pump_dsbl() {
	pump_stop();
	gpio_reset_pin(PUMP_CONTROLLER);
	p_stat.pump_en = PUMP_DISABLE;
	printf("Pump De-initialized\n");
}

void pump_start() {
	gpio_set_level(PUMP_CONTROLLER,1);
	p_stat.pump_power = PUMP_ON;
	printf("Pump Started\n");
}

void pump_stop() {
	gpio_set_level(PUMP_CONTROLLER,0);
	p_stat.pump_power = PUMP_OFF;
	printf("Pump Stopped\n");
}

void status_led_enbl() {
	gpio_set_direction(PUMP_STATUS_LED,GPIO_MODE_OUTPUT);
	status_led_off();
	printf("Pump Status LED enabled\n");
}

void status_led_dsbl() {
	status_led_off();
	gpio_reset_pin(PUMP_STATUS_LED);
	printf("Pump Status LED disabled\n");
}

void status_led_on() {
	gpio_set_level(PUMP_STATUS_LED, 1);
	printf("Pump Status LED ON\n");
}

void status_led_off() {
	gpio_set_level(PUMP_STATUS_LED, 0);
	printf("Pump Status LED OFF\n");
}

pump_status get_pump_status() {
	return p_stat;
}

pump_status pump_test() {
	// Testing whether the GPIO pin connected to pump is working fine
	// Test 1: pin pulled up, read pin level (expected - 1)
	// Test 2: pin pulled down, read pin level (expected - 0)
	// Port passes test if both the tests are passed

	gpio_set_direction(PUMP_CONTROLLER,GPIO_MODE_INPUT_OUTPUT);
	gpio_set_pull_mode(PUMP_CONTROLLER,GPIO_PULLUP_ONLY);

	int test_succ_count = 0;

	if (gpio_get_level(PUMP_CONTROLLER) == 1)
		test_succ_count += 1;

	gpio_set_pull_mode(PUMP_CONTROLLER, GPIO_PULLDOWN_ONLY);
	if (gpio_get_level(PUMP_CONTROLLER) == 0)
		test_succ_count += 1;


	gpio_reset_pin(PUMP_STATUS_LED);
	gpio_set_direction(PUMP_CONTROLLER,GPIO_MODE_OUTPUT);
	p_stat.pump_en = PUMP_ENABLE;

	printf("Pump - Port - passed %d out of %d tests\n", test_succ_count, 2);

	if (test_succ_count == 2) {
		p_stat.pump_work = PUMP_PORT_WORKING;
	}
	else {
		p_stat.pump_work = PUMP_PORT_N_WORKING;
	}

	return p_stat;
}
