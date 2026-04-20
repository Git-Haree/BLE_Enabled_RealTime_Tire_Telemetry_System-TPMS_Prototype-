idf_component_register(SRCS "main.c" "sensor_i2c.c" "ble_broadcaster.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash nimble driver)
