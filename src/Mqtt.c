/*
 * Mqtt.c
 *
 *  Created on: 03-Sep-2023
 *      Author: Chowdhary Kshitij Singh
 */

#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "typeconv.h"

static const char *TAG = "MQTT";
static bool isMqttConnected;
static bool pumpCommand;

#define CONFIG_BROKER_URL "mqtt://Kshitij_singh:aio_SFqg983weNbKwYWlsfwKiGcJbvkG@io.adafruit.com:1883"
#define TOPIC_TEMPERATURE "Kshitij_singh/feeds/temperature"
#define TOPIC_HUMIDITY    "Kshitij_singh/feeds/humidity"
#define TOPIC_MOISTURE    "Kshitij_singh/feeds/moisture"
#define TOPIC_MODE        "Kshitij_singh/feeds/mode"
#define TOPIC_PUMP		  "Kshitij_singh/feeds/pump"
#define TOPIC_PUMP_STATUS "Kshitij_singh/feeds/pumpstatus"

esp_mqtt_client_handle_t Mqtt_client_id;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    //ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    Mqtt_client_id = event->client;
    int msg_id = 0;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

    	msg_id = esp_mqtt_client_subscribe(Mqtt_client_id, TOPIC_PUMP, 1); // Subscribe pump topic
    	if (msg_id > 0)
    		ESP_LOGI(TAG, "Pump Subscribe msg_id=%d", msg_id);
    	else
    		ESP_LOGE(TAG, "Pump Subscribe Failed, msg_id=%d", msg_id);

        isMqttConnected = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        isMqttConnected = false;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        if (!memcmp(event->topic, TOPIC_PUMP, event->topic_len))
        {
        	if(event->data[0] == '0')
        		pumpCommand = false;
        	else
        		pumpCommand = true;
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        ESP_LOGI(TAG, "MQTT_EVENT_BEFORE_CONNECT");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}


void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}


void publish_temperature(float val)
{
	int msg_id;
    char data[7];

    ftoa(val, data, 2);  // Float to string conversion;
	msg_id = esp_mqtt_client_publish(Mqtt_client_id, TOPIC_TEMPERATURE, data, 0, 1, 0);
	if (msg_id > 0)
		ESP_LOGI(TAG, "Temperature Publish msg_id=%d , data = %.2f", msg_id, val);
	else
		ESP_LOGE(TAG, "Temperature Publish Failed, msg_id=%d", msg_id);
}

void publish_humidity(float val)
{
	int msg_id;
	char data[7];

    ftoa(val, data, 2);  // Float to string conversion;
	msg_id = esp_mqtt_client_publish(Mqtt_client_id, TOPIC_HUMIDITY, data, 0, 1, 0);

	if (msg_id > 0)
		ESP_LOGI(TAG, "Humidity Publish msg_id=%d , data = %.2f", msg_id, val);
	else
		ESP_LOGE(TAG, "Humidity Publish Failed, msg_id=%d", msg_id);
}

void publish_moisture(float val)
{
	int msg_id;
    char data[7];

    ftoa(val, data, 2);  // Float to string conversion;
	msg_id = esp_mqtt_client_publish(Mqtt_client_id, TOPIC_MOISTURE, data, 0, 1, 0);

	if (msg_id > 0)
		ESP_LOGI(TAG, "Moisture Publish msg_id=%d , data = %.2f", msg_id, val);
	else
		ESP_LOGE(TAG, "Moisture Publish Failed, msg_id=%d", msg_id);
}

void publish_mode(bool val)
{
	int msg_id;
	char data;

	if(val)
		data = '1';
	else
		data = '0';

	msg_id = esp_mqtt_client_publish(Mqtt_client_id, TOPIC_MODE, &data, 1, 1, 0);

	if (msg_id > 0)
		ESP_LOGI(TAG, "Mode Publish msg_id=%d , data = %d", msg_id, val);
	else
		ESP_LOGE(TAG, "Mode Publish Failed, msg_id=%d", msg_id);
}

void publish_pumpstatus(bool val)
{
	int msg_id;
	char data;

	if(val)
		data = '1';
	else
		data = '0';

	msg_id = esp_mqtt_client_publish(Mqtt_client_id, TOPIC_PUMP_STATUS, &data, 1, 1, 0);

	if (msg_id > 0)
		ESP_LOGI(TAG, "PumpStatus Publish msg_id=%d , data = %d", msg_id, val);
	else
		ESP_LOGE(TAG, "PumpStatus Publish Failed, msg_id=%d", msg_id);
}

bool get_pump_command(void)
{
	return pumpCommand;
}

static bool is_Mqtt_connected()
{
	return isMqttConnected;
}

void POST_MQTT(void)
{
	if(is_Mqtt_connected())
	{
		ESP_LOGI("POST MQTT", "MQTT connected with broker, POST Passed");
	}
	else
	{
		ESP_LOGE("POST MQTT", "MQTT Not connected with broker, POST Failed");
	}
}
