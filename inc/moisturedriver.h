/*
 * Adc.h
 *
 *  Created on: 03-Sep-2023
 *      Author:  Sidharth Raj
 */

#ifndef MAIN_Moisture_H_
#define MAIN_Moisture_H_

void Soil_moisture_init(void);
int SoilMoisture_Sensor_read_data();
void POST_SoilMoistureSensor(void);
float calcMoistPercent(int adcCurrentVal);

#endif /* MAIN_Moisture_H_ */
