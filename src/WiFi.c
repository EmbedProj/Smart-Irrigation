/*
 * WiFi.c
 *
 *  Created on: 03-Sep-2023
 *      Author: Chowdhary Kshitij Singh
 */

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"


#define WIFI_CONNECT_MAXIMUM_RETRY 		5
#define WIFI_CONNECTED_BIT 				BIT0
#define WIFI_FAIL_BIT      				BIT1

#define WIFI_AP_SSID	   "OnePlus 7 Pro"
#define WIFI_AP_PASS	   "Dhoni@008"

static const char *TAG = "WIFI Station";
static bool isGotIP = false;

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

wifi_config_t wifi_config = {
    .sta = {
        .ssid = WIFI_AP_SSID,
        .password = WIFI_AP_PASS,
        /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
         * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
         * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
     * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
         */
        .threshold.authmode = WIFI_AUTH_OPEN,
        .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
    },
};

static void Wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < WIFI_CONNECT_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Retry to connect with the AP : %s", WIFI_AP_SSID);
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"Connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        isGotIP = true;
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void Wifi_station_mode_init(void)
{
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

	ESP_LOGI(TAG, "Wifi initialization in station mode starting...");
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "Wifi initialization in station mode Finished");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to AP SSID : %s", WIFI_AP_SSID);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect with AP SSID : %s", WIFI_AP_SSID);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

static bool isWiFiGotIP(void)
{
	return isGotIP;
}

void POST_WiFi(void)
{
	if(isWiFiGotIP())
	{
		ESP_LOGI("POST WiFi", "WiFI got an IP, POST Passed");
	}
	else
	{
		ESP_LOGI("POST WiFi", "WiFI didn't got an IP, POST Failed");
	}
}
