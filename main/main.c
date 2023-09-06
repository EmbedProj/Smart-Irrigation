#include <moisturedriver.h>
#include <humiditydriver.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

void app_main(void)
{

	Soil_moisture_init();          // Soil Moisture init
	Si7021_Init();                 // Humidity Mositure init
	POST_SoilMoistureSensor();     // POST Soil Moisture Sensor
	POST_I2CHumiditySensor();      // POST Humidity Sensor
    while (true) {

        float moistPercent = SoilMoisture_Sensor_read_data();
        ESP_LOGI("ADC", "#######################################");
        ESP_LOGI("ADC", "Soil Moisture in Percentage = %f", moistPercent);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        float humidval = Read_Humidity();
        ESP_LOGI("I2C", "#######################################");
        ESP_LOGI("I2C", "Relative Humidity Value in Percentage = %f", humidval);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        float tempvalue = Read_Temperature();
        ESP_LOGI("I2C", "#######################################");
        ESP_LOGI("I2C", "Temperature in degree Celsius = %f", tempvalue);
        ESP_LOGI("I2C", "#######################################");

    }
}
