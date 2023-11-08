/*
 * humiditydriver.h
 *
 *  Created on: 03-Nov-2023
 *      Author: RajSid
 */

#ifndef INC_HTDRIVER_H_
#define INC_HTDRIVER_H_

#include "esp_err.h"

esp_err_t Si7021_Init(void);
float Read_Temperature();
float Read_Humidity();
void POST_I2CHumiditySensor();

#endif /* INC_HTYDRIVER_H_ */
