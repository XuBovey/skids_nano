/* IR protocols example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "ir_tools.h"

static const char *TAG = "usr_ir";

static rmt_channel_t example_tx_channel = RMT_CHANNEL_0;
static rmt_channel_t example_rx_channel = RMT_CHANNEL_2;

/**
 * @brief RMT Receive Task
 *
 */
static void usr_ir_rx_task(void *arg)
{
    uint8_t data0[8];
    uint8_t data1[8];
    uint8_t data0_len;
    uint8_t data1_len;
    int cnt = 0;

    size_t length = 0;

    RingbufHandle_t rb = NULL;
    rmt_item32_t *items = NULL;

    rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(CONFIG_IR_RMT_RX_GPIO, example_rx_channel);
    rmt_rx_config.rx_config.idle_threshold = 25000; // xubh important enable receive connect data
//    rmt_rx_config.rx_config.filter_ticks_thresh = 100; // xubh important
//    rmt_rx_config.rx_config.filter_en = false; // xubh important
    rmt_config(&rmt_rx_config);
    rmt_driver_install(example_rx_channel, 1000, 0); // rx buf 1000bytes
    ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)example_rx_channel);
    ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
    ir_parser_config.margin_us = 250;
    ir_parser_t *ir_parser = NULL;
#if CONFIG_IR_IR_PROTOCOL_NEC
    ir_parser = ir_parser_rmt_new_nec(&ir_parser_config);
#elif CONFIG_IR_IR_PROTOCOL_RC5
    ir_parser = ir_parser_rmt_new_rc5(&ir_parser_config);
#endif

    //get RMT RX ringbuffer
    rmt_get_ringbuf_handle(example_rx_channel, &rb);
    assert(rb != NULL);
    // Start receive
    rmt_rx_start(example_rx_channel, true);
    ESP_LOGI(TAG, "items size=%d", xRingbufferGetMaxItemSize(rb));
    while (1) {
        items = (rmt_item32_t *) xRingbufferReceive(rb, &length, portMAX_DELAY);
        if (items) {
            length /= 4; // one RMT = 4 Bytes
            ESP_LOGI(TAG, "[%d] Scan items=%d", cnt, length);
            cnt += 1;

            if (ir_parser->input(ir_parser, items, length) == ESP_OK) {
            	memset(data0,0,sizeof(data0));
            	memset(data1,0,sizeof(data1));
                if (ir_parser->get_scan_code_ext(ir_parser, &data0, &data1, &data0_len, &data1_len, (uint8_t)length) == ESP_OK) {
                	printf("--------------------------------------------------------------------------------\n");
                	for(int i=0; i < 8; i ++) {
                		for (int j = 0; j < 8; j ++) {
                			printf("%d-", data0[i] & 1<<(j)?1:0);
                		}
                	}
                	printf("\n--------------------------------------------------------------------------------\n");
                	for(int i=0; i < 8; i ++) {
                		for (int j = 0; j < 8; j ++) {
                			printf("%d-", data1[i] & 1<<(j)?1:0);
                		}
                	}
                	printf("\n--------------------------------------------------------------------------------\n");
                }
            }

			vRingbufferReturnItem(rb, (void *) items);
        }
    }
    ir_parser->del(ir_parser);
    rmt_driver_uninstall(example_rx_channel);
    vTaskDelete(NULL);
}

/**
 * @brief RMT Transmit Task
 *
 */
static void usr_ir_tx_task(void *arg)
{
    uint64_t data0 = 0x10;
    uint32_t data1 = 0x20;

    rmt_item32_t *items = NULL;
    size_t length = 0;
    ir_builder_t *ir_builder = NULL;

    rmt_config_t rmt_tx_config = RMT_DEFAULT_CONFIG_TX(CONFIG_IR_RMT_TX_GPIO, example_tx_channel);
    rmt_tx_config.tx_config.carrier_en = true;
    rmt_config(&rmt_tx_config);
    rmt_driver_install(example_tx_channel, 0, 0);
    ir_builder_config_t ir_builder_config = IR_BUILDER_DEFAULT_CONFIG((ir_dev_t)example_tx_channel);
    ir_builder_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
    ir_builder_config.buffer_size = 100;
    #if CONFIG_IR_IR_PROTOCOL_NEC
    ir_builder = ir_builder_rmt_new_nec(&ir_builder_config);
#elif CONFIG_IR_IR_PROTOCOL_RC5
    ir_builder = ir_builder_rmt_new_rc5(&ir_builder_config);
#endif
    ESP_LOGI(TAG, "start loop\n");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Send new key code
        ESP_ERROR_CHECK(ir_builder->build_frame_ext(ir_builder,(uint8_t*)&data0, (uint8_t*)&data1, 35, 32, 1));
        ESP_ERROR_CHECK(ir_builder->get_result(ir_builder, &items, &length));
        // To send data according to the waveform items.
        rmt_write_items(example_tx_channel, items, length, false);
    }
    ir_builder->del(ir_builder);
    rmt_driver_uninstall(example_tx_channel);
    vTaskDelete(NULL);
}

void usr_ir_start(void)
{
    xTaskCreate(usr_ir_rx_task, "ir_rx_task", 4096, NULL, 10, NULL);
    xTaskCreate(usr_ir_tx_task, "ir_tx_task", 4096, NULL, 10, NULL);
}
