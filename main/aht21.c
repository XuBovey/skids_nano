/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "strings.h"

static const char *TAG = "i2c-aht21";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DELAY_TIME_BETWEEN_ITEMS_MS 2000 /*!< delay time between different test items */

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */

#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */


#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0

#define AHT20_STARTUP_TIME     20 // �ϵ�����ʱ��
#define AHT20_CALIBRATION_TIME 40 // ��ʼ����У׼��ʱ��
#define AHT20_MEASURE_TIME     75 // ����ʱ��

#define AHT20_DEVICE_ADDR   0x38
#define AHT21_read_ADDR     ((0x38<<1)|0x1)
#define AHT21_write_ADDR    ((0x38<<1)|0x0)

#define AHT20_CMD_CALIBRATION       0xBE // ��ʼ����У׼������
#define AHT20_CMD_CALIBRATION_ARG0  0x08
#define AHT20_CMD_CALIBRATION_ARG1  0x00

/**
 * �������ڲɼ�ʱ��Ҫʱ��,������������ָ�0xAC������ʱ75���������ٶ�ȡת��������ݲ��жϷ��ص�״̬λ�Ƿ�������
 * ��״̬����λ[Bit7]Ϊ0�������ݿ�������ȡ��Ϊ1ʱ������Ϊæ״̬��������Ҫ�ȴ����ݴ�����ɡ�
 **/
#define AHT20_CMD_TRIGGER       0xAC // ������������
#define AHT20_CMD_TRIGGER_ARG0  0x33
#define AHT20_CMD_TRIGGER_ARG1  0x00

// ����������رպ��ٴδ򿪵�Դ������£���������������ϵͳ����λ����ʱ�䲻����20 ����
#define AHT20_CMD_RESET      0xBA // ��λ����

#define AHT20_CMD_STATUS     0x71 // ��ȡ״̬����


/**
 * STATUS ����ظ���
 * 1. ��ʼ���󴥷�����֮ǰ��STATUS ֻ�ظ� 1B ״ֵ̬��
 * 2. ��������֮��STATUS �ظ�6B�� 1B ״ֵ̬ + 2B ʪ�� + 4bʪ�� + 4b�¶� + 2B �¶�
 *      RH = Srh / 2^20 * 100%
 *      T  = St  / 2^20 * 200 - 50
 **/
#define AHT20_STATUS_BUSY_SHIFT 7       // bit[7] Busy indication
#define AHT20_STATUS_BUSY_MASK  (0x1<<AHT20_STATUS_BUSY_SHIFT)
#define AHT20_STATUS_BUSY(status) ((status & AHT20_STATUS_BUSY_MASK) >> AHT20_STATUS_BUSY_SHIFT)

#define AHT20_STATUS_MODE_SHIFT 5       // bit[6:5] Mode Status
#define AHT20_STATUS_MODE_MASK  (0x3<<AHT20_STATUS_MODE_SHIFT)
#define AHT20_STATUS_MODE(status) ((status & AHT20_STATUS_MODE_MASK) >> AHT20_STATUS_MODE_SHIFT)

                                        // bit[4] Reserved
#define AHT20_STATUS_CALI_SHIFT 3       // bit[3] CAL Enable
#define AHT20_STATUS_CALI_MASK  (0x1<<AHT20_STATUS_CALI_SHIFT)
#define AHT20_STATUS_CALI(status) ((status & AHT20_STATUS_CALI_MASK) >> AHT20_STATUS_CALI_SHIFT)
                                        // bit[2:0] Reserved

#define AHT20_STATUS_RESPONSE_MAX 6

#define AHT20_RESOLUTION            (1<<20)  // 2^20

#define AHT20_MAX_RETRY 10

float temp = 0.0, humi = 0.0;

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 * @note cannot use master read slave on esp32c3 because there is only one i2c controller on esp32c3
 */
static esp_err_t __attribute__((unused)) i2c_read(i2c_port_t i2c_num, uint8_t addr, uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 * @note cannot use master write slave on esp32c3 because there is only one i2c controller on esp32c3
 */
static esp_err_t __attribute__((unused)) i2c_write(i2c_port_t i2c_num, uint8_t addr, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t AHT21_read(uint8_t *data_wr, size_t size)
{
	return i2c_read(I2C_MASTER_NUM, AHT20_DEVICE_ADDR, data_wr, size);
}

static esp_err_t AHT21_write(uint8_t *data_wr, size_t size)
{
	return i2c_write(I2C_MASTER_NUM, AHT20_DEVICE_ADDR, data_wr, size);
}

// ���ͻ�ȡ״̬����
static esp_err_t AHT20_StatusCommand(void)
{
    uint8_t statusCmd[] = { AHT20_CMD_STATUS };
    return AHT21_write(statusCmd, sizeof(statusCmd));
}

// ������λ����
static esp_err_t AHT20_ResetCommand(void)
{
    uint8_t resetCmd[] = {AHT20_CMD_RESET};
    return AHT21_write(resetCmd, sizeof(resetCmd));
}

// ���ͳ�ʼ��У׼����
static esp_err_t AHT20_CalibrateCommand(void)
{
    uint8_t clibrateCmd[] = {AHT20_CMD_CALIBRATION, AHT20_CMD_CALIBRATION_ARG0, AHT20_CMD_CALIBRATION_ARG1};
    return AHT21_write(clibrateCmd, sizeof(clibrateCmd));
}

// ��ȡ��ʪ��ֵ֮ǰ�� ����Ҫ��״̬�ֵ�У׼ʹ��λBit[3]�Ƿ�Ϊ 1(ͨ������0x71���Ի�ȡһ���ֽڵ�״̬��)��
// �����Ϊ1��Ҫ����0xBE����(��ʼ��)������������������ֽڣ� ��һ���ֽ�Ϊ0x08���ڶ����ֽ�Ϊ0x00��
uint32_t AHT20_Calibrate(void)
{
    uint32_t retval = 0;
    uint8_t buffer[AHT20_STATUS_RESPONSE_MAX];

    bzero(buffer, sizeof(buffer));
//    memset(&buffer, 0x0, sizeof(buffer));

    retval = AHT20_StatusCommand();
    if (retval != ESP_OK) {
        return retval;
    }

    retval = AHT21_read(buffer, sizeof(buffer));
    if (retval != ESP_OK) {
        return retval;
    }

    if (AHT20_STATUS_BUSY(buffer[0]) || !AHT20_STATUS_CALI(buffer[0])) {
        retval = AHT20_ResetCommand();
        if (retval != ESP_OK) {
            return retval;
        }
//        usleep(AHT20_STARTUP_TIME);
        vTaskDelay(AHT20_STARTUP_TIME / portTICK_PERIOD_MS);
        retval = AHT20_CalibrateCommand();
//        usleep(AHT20_CALIBRATION_TIME);
        vTaskDelay(AHT20_CALIBRATION_TIME / portTICK_PERIOD_MS);
        return retval;
    }

    return ESP_OK;
}

// ���� �������� �����ʼ����
uint32_t AHT20_StartMeasure(void)
{
    uint8_t triggerCmd[] = {AHT20_CMD_TRIGGER, AHT20_CMD_TRIGGER_ARG0, AHT20_CMD_TRIGGER_ARG1};
    return AHT21_write(triggerCmd, sizeof(triggerCmd));
}

// ���ղ��������ƴ��ת��Ϊ��׼ֵ
uint32_t AHT20_GetMeasureResult(float* temp, float* humi)
{
    uint32_t retval = 0, i = 0;
    if (temp == NULL || humi == NULL) {
        return ESP_FAIL;
    }

    uint8_t buffer[AHT20_STATUS_RESPONSE_MAX];
//    memset(&buffer, 0x0, sizeof(buffer));
    bzero(buffer, sizeof(buffer));
    retval = AHT21_read(buffer, sizeof(buffer));  // recv status command result
    if (retval != ESP_OK) {
        return retval;
    }

    for (i = 0; AHT20_STATUS_BUSY(buffer[0]) && i < AHT20_MAX_RETRY; i++) {
        // printf("AHT20 device busy, retry %d/%d!\r\n", i, AHT20_MAX_RETRY);
//        usleep(AHT20_MEASURE_TIME);
        vTaskDelay(AHT20_MEASURE_TIME / portTICK_PERIOD_MS);
        retval = AHT21_read(buffer, sizeof(buffer));  // recv status command result
        if (retval != ESP_OK) {
            return retval;
        }
    }
    if (i >= AHT20_MAX_RETRY) {
        printf("AHT20 device always busy!\r\n");
        return ESP_FAIL;
    }

    uint32_t humiRaw = buffer[1];
    humiRaw = (humiRaw << 8) | buffer[2];
    humiRaw = (humiRaw << 4) | ((buffer[3] & 0xF0) >> 4);
    *humi = humiRaw / (float)AHT20_RESOLUTION * 100;

    uint32_t tempRaw = buffer[3] & 0x0F;
    tempRaw = (tempRaw << 8) | buffer[4];
    tempRaw = (tempRaw << 8) | buffer[5];
    *temp = tempRaw / (float)AHT20_RESOLUTION * 200 - 50;
     printf("humi = %05X, %f, temp= %05X, %f\r\n", humiRaw, *humi, tempRaw, *temp);
    return ESP_OK;
}

static esp_err_t aht21_demo(i2c_port_t i2c_num)
{


    AHT20_Calibrate();

    AHT20_StartMeasure();

    AHT20_GetMeasureResult(&temp, &humi);
    printf("AHT20_GetMeasureResult: temp = %.2f, humi = %.2f\r\n", temp, humi);

    return ESP_OK;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
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

static void i2c_task(void *arg)
{
    int cnt = 0;
    while (1) {
        aht21_demo(I2C_MASTER_NUM);
        vTaskDelay(DELAY_TIME_BETWEEN_ITEMS_MS/ portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void aht21_start(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    xTaskCreate(i2c_task, "i2c_task_0", 1024 * 2, (void *)0, 10, NULL);
}

void read_aht21_data(float *_temp, float *_humi)
{
	*_temp = temp;
	*_humi = humi;
}
