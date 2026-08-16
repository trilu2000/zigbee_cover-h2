#include "esp_log.h"
#include "esp_pm.h"
#include "nvs_flash.h"

//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

#include "inputparser.h"
#include "helpers.h"

#include "zigbee/zigbee.h"
#include "cover/cover.h"





static const ZigbeeConfig zigbee_config =
{
    .manufacturer = "trilu2k",
    .model        = "ESP32H2-Rollotron"
};

static const CoverConfig cover_config =
{
    .rx_pin   = GPIO_NUM_4,
    .tx_pin   = GPIO_NUM_5,
    .rstn_pin = GPIO_NUM_12,
    .uart     = UART_NUM_1,
};


static const char *TAG = "MAIN";

Cover cover;


extern "C" void app_main(void)
{
    // log settings
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("COVER", ESP_LOG_VERBOSE);
    esp_log_level_set("ZIGBEE", ESP_LOG_INFO);

    // all inits
    ESP_ERROR_CHECK(nvs_flash_init());

    parser.init(cmdTab);
    cover.init(cover_config);
    zigbee_init(zigbee_config);

    // set power management
    esp_pm_config_t pm_config = {
        .max_freq_mhz = 48,
        .min_freq_mhz = 16,
        .light_sleep_enable = true,
    };

    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    // the main loop
    while (true)
    {
        parser.poll();
        cover.poll();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}



/* -----------------------------------------------------------------------------------------------------------
 * Input parser
 */

 void show_help() 
{
    ESP_LOGI("test", "show_help");
}

void zigbee_set_open()
{
    zigbee_set_operation(ZB_OPERATION_OPENING);
}
void zigbee_set_close()
{
    zigbee_set_operation(ZB_OPERATION_CLOSING);
}

void zigbee_set_idle()
{
    zigbee_set_operation(ZB_OPERATION_IDLE);
}

void zigbee_set_position()
{
    uint8_t pos;
    parser >> pos;
    zigbee_set_position(pos);
}

void cover_set_open()
{
    cover.set_operation(CoverCommand::OPEN);
}
void cover_set_close()
{
    cover.set_operation(CoverCommand::CLOSE);
}
void cover_set_stop()
{
    cover.set_operation(CoverCommand::STOP);
}
void cover_set_position()
{
    uint8_t pos;
    parser >> pos;
    cover.set_position(pos);
}

void cmd_tx()
{
    uint8_t data[200];
    uint8_t len;

    parser >> data >> len;
    cover.send_frame(data, len);
}

const Commands cmdTab[] =
{
    { "zp", 0, zigbee_pair },
    { "zr", 0, zigbee_factory_reset },
    { "zs", 0, zigbee_status },
    { "zo", 0, zigbee_set_open },
    { "zc", 0, zigbee_set_close },
    { "zi", 0, zigbee_set_idle },
    { "zp", 1, zigbee_set_position },
    { "co", 0, cover_set_open },
    { "cc", 0, cover_set_close },
    { "cs", 0, cover_set_stop },
    { "cp", 1, cover_set_position },
    { "tx", 1, cmd_tx },
    {}
};


/* -----------------------------------------------------------------------------------------------------------
 * Zigbee Event
 */
extern "C" void on_zigbee_event(const ZigbeeEvent &event)
{
    switch (event.command)
    {
        case ZigbeeCommand::OPEN:
            ESP_LOGD(TAG, "Zigbee -> Cover: OPEN");
            cover.set_operation(CoverCommand::OPEN);
            break;

        case ZigbeeCommand::CLOSE:
            ESP_LOGD(TAG, "Zigbee -> Cover: CLOSE");
            cover.set_operation(CoverCommand::CLOSE);
            break;

        case ZigbeeCommand::STOP:
            ESP_LOGD(TAG, "Zigbee -> Cover: STOP");
            cover.set_operation(CoverCommand::STOP);
            break;

        case ZigbeeCommand::SET_POSITION:
        {   // A Zigbee cover position represents the percentage-based opening level of a motorized blind
            // zigbee value 0 - 100, 0: open, 100: close (Homeassistant)
            // Rollotron value 0 - 255, 0: close, 255: open
            uint8_t pos = (static_cast<uint16_t>(event.position) * 255) / 100;
            ESP_LOGD(TAG, "Zigbee -> Cover: SET_POSITION,  %u%% -> %d", event.position, pos);
            cover.set_position(pos);
            break;
        }
        default:
            ESP_LOGW(TAG, "Zigbee -> Cover: Unknown command");
            break;
    }
}


/* -----------------------------------------------------------------------------------------------------------
 * Zigbee Status on pairing or factory reset
 */
void on_zigbee_status(const ZigbeeStatus status) 
{
    switch (status)
    {
        case ZigbeeStatus::START_PAIR:
            ESP_LOGI(TAG, "Zigbee state: START_PAIR");
            cover.led_orange();
            break;

        case ZigbeeStatus::HAS_JOINED:
            ESP_LOGI(TAG, "Zigbee state: HAS_JOINED");
            cover.led_green();
            break;

        case ZigbeeStatus::ALREADY_JOINED:
            ESP_LOGI(TAG, "Zigbee state: ALREADY_JOINED");
            cover.led_green();
            break;

        case ZigbeeStatus::PAIR_FAILED:
            ESP_LOGI(TAG, "Zigbee state: PAIR_FAILED");
            cover.led_red();
            break;

        case ZigbeeStatus::Start_DEREGISTER:
            ESP_LOGI(TAG, "Zigbee state: Start_DEREGISTER");
            cover.led_orange();
            break;

        case ZigbeeStatus::IS_DEREGISTERED:
            ESP_LOGI(TAG, "Zigbee state: IS_DEREGISTERED");
            cover.led_red();
            break;

        default:
            ESP_LOGI(TAG, "Zigbee state: UNKNOWN");
            cover.led_orange();
            break;
    }
}


/* -----------------------------------------------------------------------------------------------------------
 * Cover Event - operation or position
 */
extern "C"
void on_cover_event(const CoverEvent &event)
{
    if (event.type == CoverEventType::OPERATION)
    {
        switch (event.operation)
        {
            case CoverOperation::OPENING:
                ESP_LOGI(TAG, "Cover -> OPENING");
                zigbee_set_operation(ZB_OPERATION_OPENING);
                break;

            case CoverOperation::CLOSING:
                ESP_LOGI(TAG, "Cover -> CLOSING");
                zigbee_set_operation(ZB_OPERATION_CLOSING);
                break;

            case CoverOperation::IDLE:
                ESP_LOGI(TAG, "Cover -> IDLE");
                zigbee_set_operation(ZB_OPERATION_IDLE);
                break;
        }
    }
    else if (event.type == CoverEventType::POSITION)
    {
        ESP_LOGI(TAG, "Cover -> POSITION %u", event.position);
        // Rollotron value 0 - 255, 0: close, 255: open
        // A Zigbee cover position represents the percentage-based opening level of a motorized blind
        // zigbee value 0 - 100, 0: open, 100: close (Homeassistant)
        uint8_t pos = (static_cast<uint16_t>(event.position) * 100) / 255;
        // in case it is inverted
        //uint8_t pos = 100 - (static_cast<uint16_t>(event.position) * 100) / 255;
        zigbee_set_position(pos);
    }
}


/* -----------------------------------------------------------------------------------------------------------
 * Cover Button event - press DECT button short or long
 */
void on_cover_button(const CoverButton button)
{
    if (button == CoverButton::SHORT) 
    {
        ESP_LOGI(TAG, "Zigbee start pair");
        zigbee_pair();
    } 
    else if (button == CoverButton::LONG)
    {
        ESP_LOGI(TAG, "Zigbee factory reset");
        zigbee_factory_reset();
    } 
}


