#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "WiFi.h"
#include "Mqtt.h"


void app_main(void)
{

    Wifi_station_mode_init(); // WiFi Init and Connect
    POST_WiFi();			  // WiFi POST

    mqtt_app_start();         // MQTT Init and Connect
    POST_MQTT();			  // MQTT POST

    while (true) {
    	publish_temperature("23.7");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        publish_humidity("79.8");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        publish_moisture("68.3");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
