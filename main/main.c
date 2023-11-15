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


void POselfTest() {
	printf("Initiating Power ON Self Test\n");

	// Testing the Pump and Button
	smart_control_test();


	printf("POST completed successfully\n");
}


void app_main(void)
{
	uint8_t timer_sec = 0;
	float moistPercent = 5.0;
	float humidPercent;
	float tempDegreeCelcius;
	bool user_command;
	operation_mode mode;
	pump_status pumpstat;


	// Initializing the Smart Controller
	smart_control_start();

	// Starting the POST
	POselfTest();

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

    while (1) {

    	if(timer_sec >= 50)  // 5 sec timer
    	{
    		if(timer_sec == 50)
    		{
    			pumpstat = get_pump_status(); // Pump Status
    			publish_pumpstatus(pumpstat.pump_power);
    		}

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
    			mode = get_operation_mode();
    			publish_mode(mode);								 // Operating Mode
    			timer_sec = 0;         // Reset the timer after one cycle
    		}
    	}

        user_command = get_pump_command();               // Get Pump command from UI
        smart_control(moistPercent, user_command);


        timer_sec++;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }


	while (1) {

		//Powering ON Pump and LED
		moistPercent = 5.0;
		smart_control(moistPercent, 0);
		printf("LED powered OFF\n");
		vTaskDelay(100 / portTICK_PERIOD_MS);


		//Powering OFF Pump and LED
//		moistPercent = 25.0;
//		smart_control(moistPercent, 0);
//		printf("LED powered ON\n");
//		vTaskDelay(100 / portTICK_PERIOD_MS);

	}

}
