#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "WiFi.h"
#include "Mqtt.h"
#include "moisturedriver.h"
#include "htdriver.h"
#include "smart_controller.h"

void app_main(void)
{
	uint8_t timer_sec = 0;
	float moistPercent;
	float humidPercent;
	float tempDegreeCelcius;
	bool pump_command;

	Wifi_station_mode_init(); // WiFi Init and Connect
	vTaskDelay(500 / portTICK_PERIOD_MS);   // Before Wifi POST, give some time it to breath.
	POST_WiFi();			  // WiFi POST

	mqtt_app_start();         // MQTT Init and Connect
	vTaskDelay(2000 / portTICK_PERIOD_MS);   // Before MQTT POST, give some time it to breath.
	POST_MQTT();			  // MQTT POST

	Soil_moisture_init();          // Soil Moisture init
	POST_SoilMoistureSensor();     // POST Soil Moisture Sensor

	Si7021_Init();                 // Humidity Mositure init
	POST_I2CHumiditySensor();      // POST Humidity Sensor

    while (true) {

    	if(timer_sec >= 50)  // 5 sec timer
    	{
    		if(timer_sec == 65 )  // 1.5 sec timer (5 + 1.5)
    		{
    			moistPercent = SoilMoisture_Sensor_read_data();  // Moisture Data
    			publish_moisture(moistPercent);
    		}

    		else if(timer_sec == 80 )  // 1.5 sec timer (6.5 + 1.5)
    		{
    			tempDegreeCelcius = Read_Temperature();          // Temperature
    			publish_temperature(tempDegreeCelcius);
    		}

    		else if(timer_sec == 95 )  // 1.5 sec timer (8 + 1.5)
    		{
    			humidPercent = Read_Humidity();					 // Humidity
    			publish_humidity(humidPercent);
    		}

    		else if(timer_sec == 110 )	// 1.5 sec timer (9.5 + 1.5)
    		{
    			publish_mode(1);								 // Operating Mode
    			timer_sec = 0;         // Reset the timer after one cycle
    		}
    	}
        pump_command = get_pump_command();               // Get Pump command from UI
        if (pump_command)
        {
        	printf("Turn On the Pump\n");
        }
        else
        {
        	printf("Turn OFF the Pump\n");
        }

        timer_sec++;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
