/*
 * usr_aip1638.c
 *
 *  Created on: 2021年8月26日
 *      Author: Bovey
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define AIP1638_HOST    VSPI_HOST
#define DMA_CHAN        2

// pin脚定义
#define PIN_NUM_MOSI    25
#define PIN_NUM_CLK     26
#define PIN_NUM_CS      27

spi_device_handle_t spi;

void spi_aip1638_init(void)
{
    esp_err_t ret;
//    spi_device_handle_t spi;
    // 总线配置
    spi_bus_config_t buscfg={
        .miso_io_num=-1, //不需要输入，并且需要输入时用的也是3线SPI，即收发用的时同一个IO口
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=32 //最大数据发送32字节
    };
    // 设备配置
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=400*1000,               //Clock out at 800 KHz
        .mode=3,                                //SPI mode 0
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .flags=SPI_DEVICE_BIT_LSBFIRST,         //LSB, default MSB
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
    };
    // 总线初始化
    //Initialize the SPI bus
    ret=spi_bus_initialize(AIP1638_HOST, &buscfg, DMA_CHAN);
    ESP_ERROR_CHECK(ret);
    // 添加设备
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(AIP1638_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

//    aip1638_update(spi);
//
//    spi_bus_remove_device(spi);
//    spi_bus_free(AIP1638_HOST);
}

void spi_aip1638_write(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));           //Zero out the transaction
    t.length=8*len;                     //Command is 8 bits
    t.tx_buffer=data;                   //The data is the cmd itself
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);                //Should have had no issues.
}


/*******************
1表示LED点亮

bit 7   6   5   4   3   2   1   0
    dp  g   f   e   d   c   b   a   hex flag
    0   0   1   1   1   1   1   1   3F  0
    0   0   0   0   0   1   1   0   06  1
    0   1   0   1   1   0   1   1   5b  2
    0   1   0   0   1   1   1   1   4f  3
    0   1   1   0   0   1   1   0   66  4
    0   1   1   0   1   1   0   1   6d  5
    0   1   1   1   1   1   0   1   7d  6
    0   0   0   0   0   1   1   1   07  7
    0   1   1   1   1   1   1   1   7F  8
    0   1   1   0   1   1   1   1   6F  9
    0   1   0   0   0   0   0   0   40  -
    1   0   0   0   0   0   0   0   80  .
    0   1   1   1   1   0   1   1   7b  A
    0   0   1   1   1   0   0   1   39  C
    0   1   1   1   1   0   0   1   79  E
    0   1   1   1   0   0   0   1   71  F
    0   1   1   1   0   1   1   0   76  H
    0   0   1   1   1   0   0   0   38  L
    0   0   1   1   1   1   1   0   3E  U
*******************/
uint8_t seg_font_table[]={0x3F, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x40, 0x80, 0x7b, 0x39, 0x79, 0x71, 0x76, 0x38, 0x3E};
uint8_t data_buf[17] = {0xc0, 0x00};

void aip1638_usr_buf_write(uint8_t offset, uint8_t seg_font)
{
	uint8_t index = 0;
    uint8_t seg_index = 0;

    if (offset > 7)
        return;
    
    if (offset < 4) {
        seg_index = 3 - offset;
    }else {
        seg_index = 11 - offset;
    }

	for(index = 0; index < 8; index ++) {
        // bit == 1
        if(seg_font & (1<<index)) { // set bit to 1
            data_buf[index*2 + 1] |= 1<<seg_index;
        }else {// set bit to 0
            data_buf[index*2 + 1] &= ~(1<<seg_index);
        }
	}
}

void aip1638_update(spi_device_handle_t spi, uint8_t *data_buf)
{
    uint8_t data_cmd = 0x40;
    uint8_t disp_cmd = 0x8F;

    spi_aip1638_write(spi, &data_cmd, 1);
    spi_aip1638_write(spi, data_buf, 17);
    spi_aip1638_write(spi, &disp_cmd, 1);
}

void seg_display_task( void * pvParameters )
{
	uint16_t count = 0;
	uint16_t tmp;
	uint8_t i;
	uint8_t offset;

	while (true) {
		count ++;

		tmp = count;
		for(i=0; i<8; i++) {
			offset = tmp%10;
			tmp = (uint16_t)tmp/10;
			aip1638_usr_buf_write(i, seg_font_table[offset]);
		}

		aip1638_update(spi, data_buf);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void aip1638_demo(void)
{
	uint8_t i;

	spi_aip1638_init();

	for(i=0; i<8; i++) {
		aip1638_usr_buf_write(i, seg_font_table[8]);
	}
	aip1638_update(spi, data_buf);

    xTaskCreate(seg_display_task, "seg_display_task", 1024, NULL, 5, NULL);
}
