#include "ble_broadcaster.h"
#include "esp_log.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"

static const char *TAG = "BLE_ADV";
static uint8_t ble_addr_type;

// Struct to pack our telemetry data into the advertisement
typedef struct __attribute__((packed)) {
    uint16_t company_id; // e.g. 0xFFFF for test
    int16_t temp_celsius_x100;
    int32_t pressure_pa;
} tpms_adv_data_t;

static void ble_app_set_addr(void) {
    ble_hs_id_infer_auto(0, &ble_addr_type);
}

// This function updates the current payload that is actively broadcasting
void ble_broadcaster_update_payload(float temperature, float pressure) {
    struct ble_hs_adv_fields fields;
    
    memset(&fields, 0, sizeof(fields));

    // Flags: LE General Discoverable Mode, BR/EDR Not Supported
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    // Pack the data
    tpms_adv_data_t sensor_data = {
        .company_id = 0xFFFF,
        .temp_celsius_x100 = (int16_t)(temperature * 100),
        .pressure_pa = (int32_t)(pressure * 100) // hPa to Pa
    };

    fields.mfg_data = (uint8_t*)&sensor_data;
    fields.mfg_data_len = sizeof(sensor_data);

    // Apply the configured fields to NimBLE
    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error setting advertisement data; rc=%d", rc);
    }
}

static void ble_app_advertise(void) {
    struct ble_gap_adv_params adv_params;

    memset(&adv_params, 0, sizeof(adv_params));
    
    // We want Non-Connectable undirected advertising to broadcast data to everyone 
    // without allowing them to interrupt the stream by connecting.
    adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    int rc = ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error enabling advertising; rc=%d", rc);
    } else {
        ESP_LOGI(TAG, "BLE Broadcasting started.");
    }
}

static void ble_app_on_sync(void) {
    ble_app_set_addr();

    // Set initial dummy payload
    ble_broadcaster_update_payload(20.0, 1013.25);

    // Start advertising
    ble_app_advertise();
}

static void host_task(void *param) {
    ESP_LOGI(TAG, "BLE Host Task Started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void ble_broadcaster_init(void) {
    nimble_port_init();
    ble_hs_cfg.sync_cb = ble_app_on_sync;
    nimble_port_freertos_init(host_task);
}
