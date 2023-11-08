/*
 * Mqtt.h
 *
 *  Created on: 03-Sep-2023
 *      Author: Chowdhary Kshitij Singh
 */

#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_

void mqtt_app_start(void);
void publish_temperature(float val);
void publish_humidity(float val);
void publish_moisture(float val);
void publish_mode(bool val);
bool get_pump_command(void);
void POST_MQTT(void);



#endif /* MAIN_MQTT_H_ */
