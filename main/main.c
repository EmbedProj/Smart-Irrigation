#include <smart_controller.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void POselfTest() {
	printf("Initiating Power ON Self Test\n");

	// Testing the Pump and Button
	smart_control_test();


	// Test for Display coming up !!!!!


	printf("POST completed successfully\n");
}

void app_main(void)
{
	// Initializing the Smart Controller
	smart_control_start();

	// Starting the POST
	POselfTest();

	// Sample code to test the functionality of smart_controller module
	double moisture = 5.0;

	while (1) {

		//Powering ON Pump and LED
		moisture = 5.0;
		smart_control(moisture, 0);
		printf("LED powered OFF\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);


		//Powering OFF Pump and LED
		moisture = 15.0;
		smart_control(moisture, 0);
		printf("LED powered ON\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);

//		if (moisture >= 5.0 && moisture <= 15.0) {
//			moisture = moisture + incre;
//		}
//		else if (moisture > 15.0) {
//			incre = -1.0;
//		}
//		else {
//			incre = 1.0;
//		}
	}
	//user_button_enbl();
	//int manual_mode = 1;
	//int button_state = 1;

    //REG_WRITE ( 0x3ff44020 ,BIT25 ) ;//Define GPIO2 as output
    //gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT);

    //while (1) {
        /* Toggle the LED state */

    	//button_state = get_button_status();

    	//if (button_state == 0) {
    		//REG_WRITE ( 0x3ff44008 ,BIT25 ) ; // GPIO2 = HIGH
    		//gpio_set_level(GPIO_NUM_25, 1);  // Set HIGH
    	//}
    	//else {
    		//REG_WRITE ( 0x3ff4400C ,BIT25 ) ; // GPIO2 = LOW
    		//gpio_set_level(GPIO_NUM_25, 0);  // Set LOW
    	//}

		//vTaskDelay(500 / portTICK_PERIOD_MS);
    //}
}
