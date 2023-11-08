/*
* Si7021.c
*
*  Created on: 06-Nov-2023
*      Author: SingKs
*/
#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c.h"

#define I2C_MASTER_SDA_IO 	21
#define I2C_MASTER_SCL_IO 	22
#define I2C_MASTER_FREQ_HZ	100000
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0

#define ESP_SLAVE_ADDR 		0x40
#define ACK_CHECK_EN        0x01
#define ACK_CHECK_DIS       0x0
//#define FW_VERSION


#define MEASURE_TEMP_NO_HOLD	0xF3
#define MEASURE_Humid_NO_HOLD	0xF5

esp_err_t Si7021_Init()
{
    int i2c_master_port = I2C_NUM_1;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t Si7021_write(i2c_port_t i2c_num, uint8_t hold_Nohold)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, hold_Nohold, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t Si7021_read(i2c_port_t i2c_num, uint8_t *data_h, uint8_t *data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, data_l, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

float Read_Temperature()
{
	uint8_t data_h = 0, data_l = 0;
	uint16_t tempcode = 0;
	float tempdegcelcius = 0;
    int ret;
    ret = Si7021_write(I2C_NUM_1, MEASURE_TEMP_NO_HOLD);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ret = Si7021_read(I2C_NUM_1, &data_h, &data_l);
	tempcode = (data_h << 8) | data_l;
	tempdegcelcius = ((175.72 * tempcode)/65536) - 46.85;
	ESP_LOGI("HT Driver", "Temperature in degree Celsius = %f", tempdegcelcius);
	return tempdegcelcius;
}

float Read_Humidity()
{
	uint8_t data_h = 0, data_l = 0;
	uint16_t rh_code = 0;
	float rhumiditypercentage = 0;
    int ret;
    ret = Si7021_write(I2C_NUM_1, MEASURE_Humid_NO_HOLD);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ret = Si7021_read(I2C_NUM_1, &data_h, &data_l);
	rh_code = (data_h << 8) | data_l;
	rhumiditypercentage = ((125 * rh_code)/65536) - 6;
	ESP_LOGI("HT Driver", "Relative Humidity Value in Percentage = %f", rhumiditypercentage);

	return rhumiditypercentage;
}

uint8_t Read_FWVer()
{
	uint8_t data=0;
	uint8_t dat[2] = {0x84, 0xb8};
	int ret;
	    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	    i2c_master_start(cmd);
	    i2c_master_write_byte(cmd, ESP_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	    i2c_master_write(cmd, dat, 2, ACK_CHECK_EN);
	    i2c_master_stop(cmd);
	    ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
	    i2c_cmd_link_delete(cmd);

	    //vTaskDelay(100 / portTICK_PERIOD_MS);
	    cmd = i2c_cmd_link_create();
	    i2c_master_start(cmd);
	    i2c_master_write_byte(cmd, ESP_SLAVE_ADDR << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
	    i2c_master_read(cmd, &data, 1, I2C_MASTER_ACK);
	    i2c_master_stop(cmd);
	    ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
	    i2c_cmd_link_delete(cmd);

		return data;
}

void POST_I2CHumiditySensor()
{
	uint8_t val = Read_FWVer();
	if(val == 0x20 || val == 0xFF )
	{
		ESP_LOGI("POST I2c", "I2c Relative Humidity Sensor Fw Version as expected, POST Passed");
	}
	else
	{
		ESP_LOGI("POST I2c", "I2c Relative Humidity Sensor Fw Version is not as expected, POST Failed");
	}
}
