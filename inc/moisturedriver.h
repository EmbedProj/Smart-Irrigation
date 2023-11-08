/*
 * Adc.h
 *
 *  Created on: 03-Sep-2023
 *      Author:  Sidharth Raj
 */

#ifndef MOISTUREDRIVER_H_
#define MOISTUREDRIVER_H_

void Soil_moisture_init(void);
float SoilMoisture_Sensor_read_data();
void POST_SoilMoistureSensor(void);

#endif /* MOISTUREDRIVER_H_ */
