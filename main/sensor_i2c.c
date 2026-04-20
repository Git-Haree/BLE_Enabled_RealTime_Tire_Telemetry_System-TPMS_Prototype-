#include "sensor_i2c.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <stdlib.h>

static const char *TAG = "I2C_SENSOR";

esp_err_t sensor_i2c_init(void) {
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);
    
    ESP_LOGI(TAG, "I2C initialized successfully on SDA:%d, SCL:%d", I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

// In a real system, you'd write a register address and read N bytes back.
// Here we simulate sensor readings around normal tire pressure ranges (e.g. 30-35 psi = ~2000-2400 hPa).
esp_err_t sensor_i2c_read_data(float *temperature, float *pressure) {
    // Simulated reading variations
    *temperature = 25.0 + ((rand() % 100) / 100.0) * 5.0; // 25 to 30 C
    *pressure = 2200.0 + ((rand() % 200) - 100.0);        // ~32 psi

    return ESP_OK;
}
