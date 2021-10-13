/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"

#include "time.h"
#include "sys/time.h"

#include "usr_led_strip.h"
#include "led_strip.h"

static const char *TAG = "usr_led_strip";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (1000)

/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void led_strip_update_init(led_strip_t *strip)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;

	for (int j = 0; j < CONFIG_STRIP_LED_NUMBER; j ++) {
		// Build RGB values
		hue = j * 360 / CONFIG_STRIP_LED_NUMBER;
		led_strip_hsv2rgb(hue, 100, 2, &red, &green, &blue);
		// Write RGB values to strip driver
		ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
	}
	// Flush RGB values to LEDs
	ESP_ERROR_CHECK(strip->refresh(strip, 100));

}

void led_strip_update_clock(led_strip_t *strip)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;

    uint16_t sec_num, min_num, hour_num = 0;

    // 1. �������-��ǰʱ�䣬ʵ������һ�������α���
	// #define	_TIME_T_ long
	// typedef	_TIME_T_ time_t;
	time_t now;

	// 3. ���������գ�ʱ�����ʽʱ�����
	struct tm timeinfo;

	// 4. ��ȡ��ǰʱ�䣬�õ���1970-1-1�����Ƶ������
	time(&now);

	// Set timezone to China Standard Time
	setenv("TZ", "CST-8", 1);
	tzset();

	// 5. ����������õ���ǰ��ʱ�䣨������-ʱ���룩
	localtime_r(&now, &timeinfo);

	sec_num 	= timeinfo.tm_sec;
	min_num 	= timeinfo.tm_min;
	hour_num 	= (uint16_t)((timeinfo.tm_hour%12)*5 + timeinfo.tm_min/12);

	for (int j = 0; j < CONFIG_STRIP_LED_NUMBER; j ++) {
		// Build RGB values
		hue = j * 360 / CONFIG_STRIP_LED_NUMBER;
		led_strip_hsv2rgb(hue, 100, 0, &red, &green, &blue);

		if((j+1)%5 == 0) {
			led_strip_hsv2rgb(60, 50, 1, &red, &green, &blue);
		}

		if((j+1)%60 == sec_num) {
			led_strip_hsv2rgb(0, 100, 5, &red, &green, &blue);
		}
		if((j+1)%60 == min_num) {
			led_strip_hsv2rgb(120, 100, 5, &red, &green, &blue);
		}
		if((j+1)%60 == hour_num) {
			led_strip_hsv2rgb(240, 100, 5, &red, &green, &blue);
		}
		// Write RGB values to strip driver
		ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
	}
	// Flush RGB values to LEDs
	ESP_ERROR_CHECK(strip->refresh(strip, 100));

}

void led_strip_task( void * pvParameters )
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));

    led_strip_update_init(strip);
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS*3));

    // Show simple rainbow chasing pattern
	ESP_LOGI(TAG, "LED Rainbow Chase Start");

	while (true) {
		led_strip_update_clock(strip);

		vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
	}
}
