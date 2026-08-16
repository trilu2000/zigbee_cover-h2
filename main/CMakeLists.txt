idf_component_register(
    SRCS
        "app_main.cpp"
        "zigbee/zigbee.cpp"
        "cover/cover.cpp"

    INCLUDE_DIRS
        "."
        "zigbee"
        "cover"
        "${PROJECT_DIR}/managed_components/espressif__esp-zigbee-lib/include"
        "${PROJECT_DIR}/managed_components/espressif__esp-zboss-lib/include"

    REQUIRES esp_timer
    REQUIRES nvs_flash
    
    PRIV_REQUIRES
        espressif__esp-zigbee-lib
        espressif__esp-zboss-lib
)