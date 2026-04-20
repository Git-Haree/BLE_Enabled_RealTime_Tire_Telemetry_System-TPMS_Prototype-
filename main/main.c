#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "sensor_i2c.h"
#include "ble_broadcaster.h"

static const char *TAG = "TPMS_MAIN";

// FreeRTOS Task to periodically read sensors and update BLE payload
void telemetry_task(void *pvParameters) {
    float temperature = 0.0;
    float pressure = 0.0;

    while (1) {
        // Read data via I2C (Simulated/Mocks underlying sensor interaction)
        if (sensor_i2c_read_data(&temperature, &pressure) == ESP_OK) {
            ESP_LOGI(TAG, "Sensor Read: Temp=%.2f C, Pres=%.2f hPa", temperature, pressure);
            
            // Update the BLE advertisement payload with the new data
            ble_broadcaster_update_payload(temperature, pressure);
        } else {
            ESP_LOGE(TAG, "Failed to read sensor data");
        }

        // Delay for 1 second (1Hz update rate for TPMS is standard)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting TPMS BLE Node initialization");

    // Initialize NVS (required for BLE)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize I2C Master interface
    ESP_ERROR_CHECK(sensor_i2c_init());

    // Initialize BLE NimBLE stack and start broadcasting
    ble_broadcaster_init();

    // Create FreeRTOS task handling the sensing loop
    xTaskCreate(telemetry_task, "telemetry_task", 4096, NULL, 5, NULL);
}
