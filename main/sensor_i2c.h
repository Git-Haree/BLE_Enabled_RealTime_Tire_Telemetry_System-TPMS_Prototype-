#ifndef SENSOR_I2C_H
#define SENSOR_I2C_H

#include "esp_err.h"

// Define I2C pin mappings
#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_NUM              0
#define I2C_MASTER_FREQ_HZ          400000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0

esp_err_t sensor_i2c_init(void);
esp_err_t sensor_i2c_read_data(float *temperature, float *pressure);

#endif // SENSOR_I2C_H
