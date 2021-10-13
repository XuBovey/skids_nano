/* HTTPS GET Example using plain mbedTLS sockets
 *
 * Contacts the howsmyssl.com API via TLS v1.2 and reads a JSON
 * response.
 *
 * Adapted from the ssl_client1 example in mbedtls.
 *
 * Original Copyright (C) 2006-2016, ARM Limited, All Rights Reserved, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD, Apache 2.0 License.
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"

#include "cJSON.h"
#include "unity.h"
#include "stdio.h"

#include "usr_lvgl.h"

#define WEB_SERVER          "api.seniverse.com"
#define WEB_PORT            "80"
#define WEB_URL             "https://api.seniverse.com/v3/weather/daily.json"

static const char *TAG = "https_request";

#define APIKEY		        "S-7cD41DQtD2xRQFm"
#define CITY		        "zhengzhou"
#define language	        "en"

//static const char *REQUEST = "GET "WEB_URL"?key="APIKEY""city"&language="language" HTTP/1.1\r\n"
//								"Host: "WEB_SERVER"\r\n"
//								"Connection: close\r\n"
//								"\r\n";
static void https_get_request(esp_tls_cfg_t cfg, char * _city)
{
    char buf[2048];
    int ret, len;

    struct esp_tls *tls = esp_tls_conn_http_new(WEB_URL, &cfg);

    if (tls != NULL) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto exit;
    }

//    memset(buf,sizeof(buf),0);
    bzero(buf, sizeof(buf));
    sprintf(buf, "GET %s?key=%s&location=%s&language=%s&unit=c&start=0&days=3 HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
    		WEB_URL, APIKEY, _city, language, WEB_SERVER);

    size_t written_bytes = 0;
    // ESP_LOGI(TAG, "NEED %d bytes written:\r\n%s", strlen(buf), buf);
    do {
        ret = esp_tls_conn_write(tls,
        						 buf + written_bytes,
								 strlen(buf) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written done", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            goto exit;
        }
    } while (written_bytes < strlen(buf));

    ESP_LOGI(TAG, "Reading HTTP response...");

    do {
        len = sizeof(buf) - 1;
        bzero(buf, sizeof(buf));
        ret = esp_tls_conn_read(tls, (char *)buf, len);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        }

        if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        }

        if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        len = ret;
        ESP_LOGI(TAG, "https %d bytes read", len);

        /* Print response directly to stdout as it is read */
        #if 0
        for (int i = 0; i < len; i++) {
            putchar(buf[i]);
        }
        putchar('\n'); // JSON output doesn't have a newline at end
        #endif

        // JSON
        char * sJsonData = strstr(buf, "{\"");
        if(NULL == sJsonData) {
        	break;
        }

        cJSON *root = cJSON_Parse(sJsonData);
        if(NULL != root) {
        	cJSON *pResults = cJSON_GetObjectItem(root, "results");
        	if(NULL == pResults) {
        		ESP_LOGE(TAG, "pResults is null for results");
        		cJSON_Delete(root);
        		break;
        	}
        	// ESP_LOGI(TAG, "array size = %d", cJSON_GetArraySize(pResults));

			pResults = cJSON_GetArrayItem(pResults, 0);
            cJSON *pLastUpdate  = cJSON_GetObjectItem(pResults, "last_update");
            ESP_LOGI(TAG, "last_update = %s", cJSON_Print(pLastUpdate));

			cJSON *pDailyArray = cJSON_GetObjectItem(pResults, "daily");
			ESP_LOGI(TAG, "array size = %d", cJSON_GetArraySize(pDailyArray));

            for(int i = 0; i < cJSON_GetArraySize(pDailyArray); i++){
                cJSON *pDaily = cJSON_GetArrayItem(pDailyArray, i);
                cJSON *pData = cJSON_GetObjectItem(pDaily, "date");
                cJSON *pTextDay = cJSON_GetObjectItem(pDaily, "text_day");
                cJSON *pCodeDay = cJSON_GetObjectItem(pDaily, "code_day");
                cJSON *pTextNight = cJSON_GetObjectItem(pDaily, "text_night");
                
                ESP_LOGI(TAG, "date = %s", pData->valuestring);
                ESP_LOGI(TAG, "text_day = %s", pTextDay->valuestring);
                ESP_LOGI(TAG, "code_day = %s", pCodeDay->valuestring);
                ESP_LOGI(TAG, "text_night = %s", pTextNight->valuestring);

                int code = atoi(pCodeDay->valuestring);
                weather_update_to_ldc(i, code);
            }

			cJSON_Delete(root);
        }else{
        	ESP_LOGE(TAG, "json data is null");
        }

        break;
    } while (1);

exit:

    esp_tls_conn_delete(tls);
//    for (int countdown = 10; countdown >= 0; countdown--) {
//        ESP_LOGI(TAG, "%d...", countdown);
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
}

static void https_request_task(void *pvparameters)
{
    ESP_LOGI(TAG, "Start https_request example");

    esp_tls_cfg_t cfg = {
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    https_get_request(cfg, CITY);

    ESP_LOGI(TAG, "Finish https_request example");
    vTaskDelete(NULL);
}

void https_request_weather(void)
{
    xTaskCreate(&https_request_task, "https_get_task", 8192, NULL, 5, NULL);
}
