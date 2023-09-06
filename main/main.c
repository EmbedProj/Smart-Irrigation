#include <moisturedriver.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{

	Soil_moisture_init(); // Soil Moisture init
	POST_SoilMoistureSensor();    // POST Soil Moisture Sensor

    while (true) {

        int val = SoilMoisture_Sensor_read_data();
        ESP_LOGI("ADC", "#######################################");
        ESP_LOGI("ADC", "Soil Moisture Value = %d", val);
        float moistPercent = calcMoistPercent(val);
        ESP_LOGI("ADC", "***************************************");
        ESP_LOGI("ADC", "Soil Humidity in Percentage = %f", moistPercent);
        ESP_LOGI("ADC", "#######################################");
        vTaskDelay(5000 / portTICK_PERIOD_MS);

    }
}
