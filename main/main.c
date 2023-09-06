#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Adc.h"

void app_main(void)
{

	ADC_soil_moisture_init(); // ADC init
	POST_adcSoilMoisture();              // POST ADC

    while (true) {

        int val = ADC_get_data();
        ESP_LOGI("ADC", "Soil Moisture Value = %d", val);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

    }
}
