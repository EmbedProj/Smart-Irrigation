/*
 * peripheral_list.h
 *
 *  Created on: 04-Sep-2023
 *      Author: Phani Kiran Maddukur
 */

#ifndef INC_PERIPHERAL_LIST_H_
#define INC_PERIPHERAL_LIST_H_

#include "driver/gpio.h"


#define PERIPHERAL_ON	1
#define PERIPHERAL_OFF	0

#define PERIPHERAL_WORKING 1
#define PERIPHERAL_NOT_WORKING 0

#define PERIPHERAL_ENABLE	1
#define PERIPHERAL_DISABLE	0

#define PUSH_BUTTON_GPIO_PIN	GPIO_NUM_12
#define PUMP_CONTROL_GPIO_PIN	GPIO_NUM_25
#define PUMP_STATUS_LED_GPIO_PIN	GPIO_NUM_26

typedef enum {
	USER_BUTTON = PUSH_BUTTON_GPIO_PIN,
	PUMP_CONTROLLER = PUMP_CONTROL_GPIO_PIN,
	PUMP_STATUS_LED = PUMP_STATUS_LED_GPIO_PIN,
} gpio_peripheral_list;



#endif /* INC_PERIPHERAL_LIST_H_ */
