/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "time.h"
#include "sys/time.h"

#include "wifi_init.h"
#include "usr_sntp.h"
#include "https_request_weather.h"
#include "smart_config.h"
#include "usr_led_strip.h"
#include "usr_aip1638.h"
#include "aht21.h"
#include "usr_lvgl.h"
#include "usr_ledc.h"

#define TAG "main"

void disp_update(void)
{
	// 1. 定义变量-当前时间，实际上是一个长整形变量
	// #define	_TIME_T_ long
	// typedef	_TIME_T_ time_t;
	time_t now;

	// 2. 定义字符数组用于存放日期字符
	char strftime_buf[64];

	// 3. 定义年月日，时分秒格式时间变量
	struct tm timeinfo;

	// 4. 获取当前时间，得到从1970-1-1到限制的秒计数
	// time_t	   time (time_t *_timer);
	time(&now);

	// Set timezone to China Standard Time
	setenv("TZ", "CST-8", 1);
	tzset();

	// 5. 根据秒计数得到当前的时间（年月日-时分秒）
	localtime_r(&now, &timeinfo);
	// 6. 将年月日转换为字符串
	strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d\n%I:%M:%S", &timeinfo);

	time_update_to_lcd(strftime_buf);

	float temp, humi;
	read_aht21_data(&temp, &humi);
	temp_update_to_lcd(temp);
	humi_update_to_lcd(humi);
}

void usr_task1( void * pvParameters )
{
	for( ;; ){
		disp_update();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void usr_sntp_task( void * pvParameters )
{
	for( ;; ){
		if (0 == wifi_is_connect())
		{
			usr_sntp_init();
		}
		vTaskDelay(30000 / portTICK_PERIOD_MS);
	}
}

void usr_request_weather_task( void * pvParameters )
{
	for( ;; ){
		if (0 == wifi_is_connect())
		{
			https_request_weather();
		}
		vTaskDelay(300000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
	// Start lvgl and gui
	usr_lvgl();

	// wifi init
	wifi_init();

#if 0
	// not use
	smart_config();
#endif
	// 7segs led demo
	aip1638_demo();

	// 
	aht21_start();

	// task for sntp get time from internet
	xTaskCreate(usr_sntp_task, "usr_sntp_task", 4096, NULL, 5, NULL);
	// task for get weather data from xinzhitianqi (https://seniverse.yuque.com/) 
	xTaskCreate(usr_request_weather_task, "usr_request_weather_task", 4096, NULL, 5, NULL);
	// task for ws2812 to display clock
	xTaskCreate(led_strip_task, "led_strip_task", 4096, NULL, 5, NULL);
	// task for update time temp him data to lcd
	xTaskCreate(usr_task1, "usr_task1", 4096, NULL, 5, NULL);
	// led blink demo
	usr_led();
}
