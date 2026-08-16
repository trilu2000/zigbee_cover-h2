#include "zigbee.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_zigbee_core.h"
#include "nwk/esp_zigbee_nwk.h"

#include "esp_zigbee_cluster.h"
#include "ha/esp_zigbee_ha_standard.h"

#define COVER_ENDPOINT 1

#ifndef ESP_ZB_HA_WINDOW_COVERING_DEVICE_ID
#define ESP_ZB_HA_WINDOW_COVERING_DEVICE_ID 0x0202 
#endif

#define ESP_ZB_ZCL_ATTR_WINDOW_COVERING_OPERATIONAL_STATE_ID 0x0022

#define TAG "ZIGBEE"



static uint8_t manufacturer_name[33];
static uint8_t model_identifier[33];

static uint8_t zb_position = 0;
static uint8_t zb_operation = 0;




const char *zigbee_operation_to_string(uint8_t operation)
{
    switch (operation)
    {
        case 0x00: return "IDLE";
        case 0x01: return "OPENING";
        case 0x02: return "CLOSING";
        default:   return "UNKNOWN";
    }
}


static uint8_t *zcl_string(char *dst, const char *src)
{
    size_t len = strlen(src);

    if (len > 32) len = 32;

    dst[0] = static_cast<uint8_t>(len);
    memcpy(&dst[1], src, len);

    return reinterpret_cast<uint8_t *>(dst);
}


static void zigbee_task(void *arg)
{
    // some settings for the zigbee stack
    esp_zb_sleep_enable(false);
    esp_zb_start(false);
    esp_zb_set_rx_on_when_idle(true);
    ESP_LOGI(TAG, "Zigbee stack started");
    
    // the main loop for zigbee operations
    esp_zb_stack_main_loop();
    vTaskDelete(nullptr);
}


static esp_err_t zigbee_action_handler( esp_zb_core_action_callback_id_t callback_id, const void *message)
{
    if(callback_id == ESP_ZB_CORE_WINDOW_COVERING_MOVEMENT_CB_ID)
    {
        auto *cmd = static_cast<const esp_zb_zcl_window_covering_movement_message_t *>(message);
        ZigbeeEvent event{};

        switch(cmd->command)
        {
            case ESP_ZB_ZCL_CMD_WINDOW_COVERING_UP_OPEN:
                event.command = ZigbeeCommand::OPEN;
                break;

            case ESP_ZB_ZCL_CMD_WINDOW_COVERING_DOWN_CLOSE:
                event.command = ZigbeeCommand::CLOSE;
                break;

            case ESP_ZB_ZCL_CMD_WINDOW_COVERING_STOP:
                event.command = ZigbeeCommand::STOP;
                break;

            case ESP_ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE:
                event.command = ZigbeeCommand::SET_POSITION;
                event.position = cmd->payload.percentage_lift_value;
                break;

            default:
                return ESP_OK;
        }
        on_zigbee_event(event);
    }
    return ESP_OK;
}


extern "C" __attribute__((weak))
void on_zigbee_event(const ZigbeeEvent &event)
{
    ESP_LOGI(TAG, "on_zigbee_event");
}

extern "C" __attribute__((weak))
void on_zigbee_status(const ZigbeeStatus status) 
{
    ESP_LOGI(TAG, "on_zigbee_status");
}


void zigbee_init(const ZigbeeConfig &config)
{
    ESP_LOGI(TAG, "Initializing Zigbee");

    zcl_string((char *)manufacturer_name, config.manufacturer);
    zcl_string((char *)model_identifier, config.model);

    esp_zb_cfg_t zb_cfg = {};
    zb_cfg.esp_zb_role = ESP_ZB_DEVICE_TYPE_ROUTER;
//    zb_cfg.esp_zb_role = ESP_ZB_DEVICE_TYPE_ED;
    zb_cfg.install_code_policy = false;

    esp_zb_init(&zb_cfg);
    ESP_LOGI(TAG, "Zigbee role=%d", zb_cfg.esp_zb_role);

    // Endpoint list
    esp_zb_ep_list_t *ep_list = esp_zb_ep_list_create();

    // Cluster list
    esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();

    // Basic cluster
    esp_zb_basic_cluster_cfg_t basic_cfg = {
        .zcl_version = 8,
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE
    };
    esp_zb_attribute_list_t *basic = esp_zb_basic_cluster_create(&basic_cfg);

    esp_zb_basic_cluster_add_attr(
        basic,
        ESP_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID,
        manufacturer_name
    );

    esp_zb_basic_cluster_add_attr(
        basic,
        ESP_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID,
        model_identifier
    );

    esp_zb_cluster_list_add_basic_cluster(
        cluster_list,
        basic,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE
    );

    // Identify cluster
    esp_zb_identify_cluster_cfg_t identify_cfg = { 
        .identify_time = 0 
    };
    esp_zb_attribute_list_t *identify = esp_zb_identify_cluster_create(&identify_cfg);

    esp_zb_cluster_list_add_identify_cluster( cluster_list, identify, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE );
    
    // Window Covering cluster
    esp_zb_window_covering_cluster_cfg_t cover_cfg = {
        .covering_type = 0x00,
        .covering_status = 0,
        .covering_mode = 0
    };

    esp_zb_attribute_list_t *cover_cluster =
        esp_zb_window_covering_cluster_create(&cover_cfg);


       esp_err_t attr_result = esp_zb_cluster_add_attr(
        cover_cluster,
        ESP_ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
        ESP_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID,
        ESP_ZB_ZCL_ATTR_TYPE_U8,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_ONLY | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &zb_position
    );

    ESP_LOGI(TAG, "Add position attribute result=%d", attr_result);


    esp_err_t op_attr_result = esp_zb_cluster_add_attr(
        cover_cluster,
        ESP_ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
        ESP_ZB_ZCL_ATTR_WINDOW_COVERING_OPERATIONAL_STATE_ID,
        ESP_ZB_ZCL_ATTR_TYPE_8BIT_ENUM,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_ONLY | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &zb_operation
    );

    ESP_LOGI(TAG, "Add operational attribute result=%d", op_attr_result);


    esp_zb_cluster_list_add_window_covering_cluster(
        cluster_list,
        cover_cluster,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE
    );

 
    // Endpoint
    esp_zb_endpoint_config_t endpoint_config = {
        .endpoint = COVER_ENDPOINT,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
        .app_device_id = ESP_ZB_HA_WINDOW_COVERING_DEVICE_ID,
        .app_device_version = 0
    };
    esp_zb_ep_list_add_ep( ep_list, cluster_list, endpoint_config );

 
    // Register action handler
    esp_zb_core_action_handler_register(zigbee_action_handler); 
    ESP_LOGI(TAG, "Action handler registered");


    // Register device
    esp_zb_device_register(ep_list);

    ESP_LOGI(TAG, "Zigbee device registered");

    // start an own stack for running the zigbee loop
    xTaskCreate( zigbee_task, "zigbee",  4096, nullptr, 5, nullptr );
}


void zigbee_pair()
{
    esp_zb_lock_acquire(portMAX_DELAY);

    if (esp_zb_bdb_dev_joined())
    {
        ESP_LOGI(TAG, "Already joined Zigbee network");
        on_zigbee_status(ZigbeeStatus::ALREADY_JOINED);
        return;
    }

    ESP_LOGI(TAG, "Starting Zigbee pairing");
    esp_err_t err = esp_zb_bdb_start_top_level_commissioning( ESP_ZB_BDB_MODE_NETWORK_STEERING );

    esp_zb_lock_release();

    on_zigbee_status( err == ESP_OK ? ZigbeeStatus::START_PAIR : ZigbeeStatus::PAIR_FAILED);

    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Pairing start failed: %s", esp_err_to_name(err));
    }
}


void zigbee_factory_reset()
{
    ESP_LOGW(TAG, "Reset Zigbee configuration");
 
    on_zigbee_status(ZigbeeStatus::Start_DEREGISTER);
    esp_zb_bdb_reset_via_local_action();
    vTaskDelay(pdMS_TO_TICKS(1000));

    on_zigbee_status(ZigbeeStatus::IS_DEREGISTERED);
    vTaskDelay(pdMS_TO_TICKS(500));
    esp_restart();
}


void zigbee_status()
{
    ESP_LOGI(TAG, "Joined: %s", zigbee_is_joined() ? "yes" : "no");
    ESP_LOGI(TAG, "Factory new: %s", zigbee_is_factory_new() ? "yes" : "no");
}


bool zigbee_is_joined()
{
    esp_zb_lock_acquire(portMAX_DELAY);
    bool result = esp_zb_bdb_dev_joined();
    esp_zb_lock_release();
    return result;
}


bool zigbee_is_factory_new()
{
    esp_zb_lock_acquire(portMAX_DELAY);
    bool result = esp_zb_bdb_is_factory_new();
    esp_zb_lock_release();
    return result;
}


void zigbee_set_position(uint8_t position)
{
    if (position > 100) position = 100;
    zb_position = position;
    esp_zb_lock_acquire(portMAX_DELAY);

    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        COVER_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID,
        &zb_position,
        false
    );

    ESP_LOGI(TAG, "Set position %d%% result=%d", zb_position, status);
    esp_zb_lock_release();

    if (status == ESP_ZB_ZCL_STATUS_SUCCESS)
        zigbee_report_attribute( ESP_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID );
}


void zigbee_set_operation(uint8_t operation)
{
    // ESP_ZB_ZCL_ATTR_WINDOW_COVERING_OPERATIONAL_STATE_ID
    zb_operation = operation;
    esp_zb_lock_acquire(portMAX_DELAY);

    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        COVER_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_WINDOW_COVERING_OPERATIONAL_STATE_ID,
        &zb_operation,
        false
    );

    ESP_LOGI(TAG, "Set operation %s, result=%d", zigbee_operation_to_string(zb_operation), status);
    esp_zb_lock_release();

    if (status == ESP_ZB_ZCL_STATUS_SUCCESS)
        zigbee_report_attribute( ESP_ZB_ZCL_ATTR_WINDOW_COVERING_OPERATIONAL_STATE_ID );
}


void zigbee_report_attribute(uint16_t attribute_id)
{
    esp_zb_zcl_report_attr_cmd_t report_cmd = {};

    report_cmd.address_mode = ESP_ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    report_cmd.clusterID = ESP_ZB_ZCL_CLUSTER_ID_WINDOW_COVERING;
    report_cmd.attributeID = attribute_id;
    report_cmd.manuf_code = 0;

    report_cmd.zcl_basic_cmd.src_endpoint = COVER_ENDPOINT;
    report_cmd.zcl_basic_cmd.dst_endpoint = 0x00;

    report_cmd.direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_CLI;
    report_cmd.dis_default_resp = 1;

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_err_t err = esp_zb_zcl_report_attr_cmd_req(&report_cmd);
    esp_zb_lock_release();
    ESP_LOGI(TAG, "Report result=%s", esp_err_to_name(err));
}


extern "C" void esp_zb_app_signal_handler( esp_zb_app_signal_t *signal_struct )
{

    esp_zb_app_signal_type_t sig_type = (esp_zb_app_signal_type_t) *signal_struct->p_app_signal;
    uint32_t *p_err = signal_struct->p_app_signal;

    ESP_LOGI(TAG, "ZB signal 0x%x status=%d", sig_type, signal_struct->esp_err_status);

    switch(sig_type)
    {
        case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
        {
            ESP_LOGI(TAG, "Zigbee stack startup");

            if (esp_zb_bdb_is_factory_new())
            {
                ESP_LOGI(TAG, "Factory new device");
            }
            else
            {
                ESP_LOGI(TAG, "Network configuration found");

                esp_zb_bdb_start_top_level_commissioning(
                    ESP_ZB_BDB_MODE_INITIALIZATION
                );
            }

            break;
        }

        case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
        {
            ESP_LOGI(TAG, "First start");

            esp_zb_bdb_start_top_level_commissioning(
                ESP_ZB_BDB_MODE_NETWORK_STEERING
            );

            break;
        }

        case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
        {
            ESP_LOGI(TAG, "Device reboot/rejoin");

            if (esp_zb_bdb_dev_joined())
            {
                on_zigbee_status(ZigbeeStatus::HAS_JOINED);
            }

            break;
        }

//        case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
//        {
//            ESP_LOGI(TAG, "Zigbee stack startup");
//            if (esp_zb_bdb_is_factory_new())
//            {
//                ESP_LOGI(TAG, "Factory new device - waiting for pairing");
//            }
//            else
//            {
//                ESP_LOGI(TAG, "Network configuration found");
//                esp_zb_bdb_start_top_level_commissioning(
//                    ESP_ZB_BDB_MODE_INITIALIZATION
//                );
//            }
//            break;
//        }

//        case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
//        {
//            ESP_LOGI(TAG, "First start factory_new=%d", esp_zb_bdb_is_factory_new());
//
//            if (esp_zb_bdb_is_factory_new())
//            {
//                esp_zb_bdb_start_top_level_commissioning(
//                    ESP_ZB_BDB_MODE_NETWORK_STEERING
//                );
//            }
//            break;
//        }

//        case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
//        {
//            ESP_LOGI(TAG, "Device reboot/rejoin=%d", esp_zb_bdb_dev_joined());
//            break;
//        }

        case ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE:
        {   ESP_LOGI(TAG, "Device announce received");
            break;
        }

        case ESP_ZB_BDB_SIGNAL_STEERING:
        {
            esp_err_t status = (esp_err_t)*p_err;
            ESP_LOGI(TAG, "Joined=%d FactoryNew=%d", esp_zb_bdb_dev_joined(), esp_zb_bdb_is_factory_new());
            if (esp_zb_bdb_dev_joined())
            {
                ESP_LOGI(TAG, "Successfully joined Zigbee network");
                on_zigbee_status(ZigbeeStatus::HAS_JOINED);

            }
            else
            {
                ESP_LOGW(TAG, "Pairing failed: %s", esp_err_to_name(status));
                on_zigbee_status(ZigbeeStatus::PAIR_FAILED);
            }
            break;
        }

        case ESP_ZB_COMMON_SIGNAL_CAN_SLEEP:
        {
            static uint32_t sleep_count = 0;

            sleep_count++;

            if ((sleep_count % 100) == 0) {
                ESP_LOGI(TAG, "CAN_SLEEP count=%lu", sleep_count);
            }

            break;
        }

        default:
        {
            ESP_LOGD(TAG, "Zigbee signal: %d", sig_type);
            break;
        }
    
    }
}

