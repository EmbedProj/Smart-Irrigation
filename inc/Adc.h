/*
 * Adc.h
 *
 *  Created on: 03-Sep-2023
 *      Author:  Sidharth Raj
 */

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_

void ADC_soil_moisture_init(void);
int ADC_get_data();
void POST_adcSoilMoisture(void);

#endif /* MAIN_ADC_H_ */
