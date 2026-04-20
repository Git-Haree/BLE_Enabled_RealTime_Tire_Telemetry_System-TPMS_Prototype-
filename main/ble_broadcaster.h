#ifndef BLE_BROADCASTER_H
#define BLE_BROADCASTER_H

#include <stdint.h>

void ble_broadcaster_init(void);
void ble_broadcaster_update_payload(float temperature, float pressure);

#endif // BLE_BROADCASTER_H
