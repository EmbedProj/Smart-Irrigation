/*
 * Adc.c
 *
 *  Created on: 03-Sep-2023
 *      Author: Sidharth Raj
 */
#include <driver/adc.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define MIN_ADC_VALUE    0
#define MAX_ADC_VALUE    3227
#define WATER_ADC_VALUE  1400
#define MAX_PERCENTAGE   100

static bool adcValueinRange = false;

void Soil_moisture_init(void)
{
	ESP_LOGI("ADC", "Soil Moisture Initilization Started");
	ESP_LOGI("ADC", "WIDTH BIT 12");

    adc1_config_width(ADC_WIDTH_BIT_12); // ADC 12 bit

    /* Here we have used the ADC Attenuation 3
     * it will give the maximaum value 2600mv */
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11); // ADC1 Channel 0
}

int SoilMoisture_Sensor_read_data()
{
	int val= adc1_get_raw(ADC1_CHANNEL_0); // get data at ADC1 channel 0
	return val;
}

static bool checkADCValueRange(int adcValue)
{
	// max adc value will be 3227, calculation on the basis of MaxVin=2.6 volt
	if(adcValue>MIN_ADC_VALUE && adcValue<MAX_ADC_VALUE)
		adcValueinRange = true ;

	return adcValueinRange;
}

void POST_SoilMoistureSensor()
{
	int val = SoilMoisture_Sensor_read_data();
	if(checkADCValueRange(val))
	{
		ESP_LOGI("POST Adc", "ADC Soil Moisture Value is in Range, POST Passed");
	}
	else
	{
		ESP_LOGI("POST Adc", "ADC Soil Moisture Value is Out of Range, POST Failed");
	}
}

float calcMoistPercent(int adcCurrentVal)
{
	float val= adcCurrentVal - WATER_ADC_VALUE;
	float dryPercent= (val/(MAX_ADC_VALUE - WATER_ADC_VALUE)) * 100;
	float moistPercent= MAX_PERCENTAGE - dryPercent;

	return moistPercent;
}
