//#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "WiFi.h"
#include "Mqtt.h"


void app_main(void)
{

    Wifi_station_mode_init(); // WiFi Init and Connect
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Before Wifi POST, give some time it to breath.
    POST_WiFi();			  // WiFi POST

    mqtt_app_start();         // MQTT Init and Connect
    vTaskDelay(2000 / portTICK_PERIOD_MS);   // Before MQTT POST, give some time it to breath.
    POST_MQTT();			  // MQTT POST

    while (true) {
    	publish_temperature((rand()%80));
    	vTaskDelay(2000 / portTICK_PERIOD_MS);
        publish_humidity((rand()%80));
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        publish_moisture((rand()%80));
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        publish_mode(1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if(get_pump_command())
        	printf("PUMP ON \n");
		else
			printf("PUMP OFF \n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
