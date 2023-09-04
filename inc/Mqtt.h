/*
 * Mqtt.h
 *
 *  Created on: 03-Sep-2023
 *      Author: Chowdhary Kshitij Singh
 */

#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_

void mqtt_app_start(void);
void publish_temperature(char *data);
void publish_humidity(char *data);
void publish_moisture(char *data);
void publish_mode(char *data);
void get_pump(char *data);
void POST_MQTT(void);



#endif /* MAIN_MQTT_H_ */
