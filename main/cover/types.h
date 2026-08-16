#pragma once

#include <stdint.h>
#include <stddef.h>

#include "helpers.h"


namespace SrvType 
{
enum SrvType : uint16_t
{
    // Generic Services
    GENERAL                               = 0x0000,
    DEVICE_MANAGEMENT                     = 0x0001,
    IDENTIFY                              = 0x0004,
    ATTRIBUTE_REPORTING                   = 0x0006,

    // Functional Services
    ALERT                                 = 0x0100,
    TAMPER                                = 0x0101,
    DETECTOR_PROBLEM_ALERT                = 0x0102,
    BATTERY                               = 0x0103,
    KEEP_ALIVE                            = 0x0104,
    ARM_DISARM                            = 0x0105,
    ON_OFF                                = 0x0106,
    FUN                                   = 0x0108,
    DEBUG                                 = 0x0109,
    KEY_PRESS                             = 0x010A,

    // System Services
    SYSTEM                                = 0x0201,
    TECHNICIAN                            = 0x0202,
    PARAMETERS                            = 0x0203,
    SLEEP                                 = 0x0204,
    MANUFACTURE_CONFIGURATION             = 0x0206,
    VOICE_CALL                            = 0x020A,
    RODUCTION                             = 0x020B,
    SUOTA                                 = 0x020C,
    CERTIFICATION                         = 0x020D,
    REMOTE_CONTROL                        = 0x020E,
    SUOTA_PROPRIETARY                     = 0x020F
};
} // namespace SRV


namespace MsgType
{
enum GeneralMsg : uint8_t
{
    GENERAL_SET_ATTRIB_REQ                = 0x01,
    GENERAL_SET_ATTRIB_RES                = 0x02,
    GENERAL_GET_ATTRIB_REQ                = 0x03,
    GENERAL_GET_ATTRIB_RES                = 0x04,
    GENERAL_HELLO_IND                     = 0x05,
    GENERAL_ERROR_IND                     = 0x06,
    GENERAL_LINK_CFM                      = 0x07,
    GENERAL_GET_STATUS_REQ                = 0x08,
    GENERAL_GET_STATUS_RES                = 0x09,
    GENERAL_HELLO_REQ                     = 0x0A,
    GENERAL_GET_VERSION_REQ               = 0x0B,
    GENERAL_GET_VERSION_RES               = 0x0C,
    GENERAL_TRANSACTION_START_REQ         = 0x0D,
    GENERAL_TRANSACTION_START_CFM         = 0x0E,
    GENERAL_TRANSACTION_END_REQ           = 0x0F,
    GENERAL_TRANSACTION_END_CFM           = 0x10,
    GENERAL_LINK_MAINTAIN_START_REQ       = 0x11,
    GENERAL_LINK_MAINTAIN_START_CFM       = 0x12,
    GENERAL_LINK_MAINTAIN_STOP_REQ        = 0x13,
    GENERAL_LINK_MAINTAIN_STOP_CFM        = 0x14,
    GENERAL_LINK_MAINTAIN_STOPPED_IND     = 0x15,
    GENERAL_LOG                           = 0xFE,
    GENERAL_WAKEUP_REQ                    = 0xFF
};

enum DeviceManagementMsg : uint8_t
{
    DEV_MGNT_GET_ATTRIB_REQ               = 0x01,
    DEV_MGNT_GET_ATTRIB_RES               = 0x02,
    DEV_MGNT_REGISTER_DEVICE_REQ          = 0x03,
    DEV_MGNT_REGISTER_DEVICE_CFM          = 0x04,
    DEV_MGNT_REGISTER_DEVICE_IND          = 0x05,
    DEV_MGNT_DEREGISTER_DEVICE_REQ        = 0x06,
    DEV_MGNT_DEREGISTER_DEVICE_CFM        = 0x07,
    DEV_MGNT_DEREGISTER_DEVICE_IND        = 0x08
};

enum IdentifyMsg : uint8_t
{
    IDENTIFY_DEVICE_REQ                   = 0x01,
    IDENTIFY_DEVICE_RES                   = 0x02,
    IDENTIFY_SELF_REQ                     = 0x03
};

enum AttributeReportingMsg : uint8_t
{
    ATTRREP_CREATE_PERIODIC_REPORT_REQ    = 0x01,
    ATTRREP_CREATE_PERIODIC_REPORT_RES    = 0x02,
    ATTRREP_CREATE_EVENT_REPORT_REQ       = 0x03,
    ATTRREP_CREATE_EVENT_REPORT_RES       = 0x04,
    ATTRREP_ADDENTRY_PERIODIC_REQ         = 0x05,
    ATTRREP_ADDENTRY_PERIODIC_RES         = 0x06,
    ATTRREP_ADDENTRY_EVENT_REQ            = 0x07,
    ATTRREP_ADDENTRY_EVENT_RES            = 0x08,
    ATTRREP_DELETE_REPORT_REQ             = 0x09,
    ATTRREP_DELETE_REPORT_RES             = 0x0A,
    ATTRREP_PERIODIC_REPORT_NOTIFICATION_IND = 0x0B,
    ATTRREP_EVENT_REPORT_NOTIFICATION_IND    = 0x0C,
    ATTRREP_GET_PERIODIC_REPORT_ENTRIES_REQ  = 0x0D,
    ATTRREP_GET_PERIODIC_REPORT_ENTRIES_RES  = 0x0E,
    ATTRREP_GET_EVENT_REPORT_ENTRIES_REQ  = 0x0F,
    ATTRREP_GET_EVENT_REPORT_ENTRIES_RES  = 0x10,
    ATTRREP_ADD_REPORT_IND                = 0x11,
    ATTRREP_ADD_REPORT_RES                = 0x12,
    ATTRREP_REPORT_NOTIFICATION_REQ       = 0x13,
    ATTRREP_DELETE_REPORT_IND             = 0x14,
    ATTRREP_GET_REPORT_VALUES_IND         = 0x15,
    ATTRREP_GET_REPORT_VALUES_RES         = 0x16,
    ATTRREP_ADD_REPORT_REQ                = 0x17,
    ATTRREP_ADD_REPORT_CFM                = 0x18,
    ATTRREP_GET_ATTRIB_REQ                = 0x19,
    ATTRREP_GET_ATTRIB_RES                = 0x1A,
    ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_REQ = 0x1B,
    ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_RES = 0x1C
};

enum TamperAlertMsg : uint8_t
{
    TAMPER_ALERT_GET_ATTRIB_REQ             = 0x01,
    TAMPER_ALERT_GET_ATTRIB_RES             = 0x02,
    TAMPER_ALERT_NOTIFY_STATUS_REQ          = 0x03,
    TAMPER_ALERT_NOTIFY_STATUS_RES          = 0x04,
    TAMPER_ALERT_GET_ATTRIB_PACK_REQ        = 0x05,
    TAMPER_ALERT_GET_ATTRIB_PACK_RES        = 0x06,
    TAMPER_ALERT_GET_ATTRIB_DYN_PACK_REQ    = 0x07,
    TAMPER_ALERT_GET_ATTRIB_DYN_PACK_RES    = 0x08
};

enum KeepAliveMsg : uint8_t
{
    KEEP_ALIVE_I_AM_ALIVE_REQ               = 0x01,
    KEEP_ALIVE_I_AM_ALIVE_RES               = 0x02,
    KEEP_ALIVE_GET_ATTRIB_REQ               = 0x03,
    KEEP_ALIVE_GET_ATTRIB_RES               = 0x04,
    KEEP_ALIVE_SET_ATTRIB_REQ               = 0x05,
    KEEP_ALIVE_SET_ATTRIB_RES               = 0x06,
    KEEP_ALIVE_SET_ATTRIB_REQ_WITH_RES      = 0x07,
    KEEP_ALIVE_I_AM_ALIVE_WITH_RSSI_REQ     = 0x08
};

enum OnOffMsg : uint8_t
{
    ONOFF_ON_REQ                            = 0x01,
    ONOFF_ON_RES                            = 0x02,
    ONOFF_OFF_REQ                           = 0x03,
    ONOFF_OFF_RES                           = 0x04,
    ONOFF_TOGGLE_REQ                        = 0x05,
    ONOFF_TOGGLE_RES                        = 0x06,
    ONOFF_GET_ATTRIB_REQ                    = 0x07,
    ONOFF_GET_ATTRIB_RES                    = 0x08,
    ONOFF_SET_ATTRIB_REQ                    = 0x09,
    ONOFF_SET_ATTRIB_RES                    = 0x0A,
    ONOFF_SET_ATTRIB_REQ_WITH_RES           = 0x0B
};

enum FunMsg : uint8_t
{
    FUN_SEND_REQ                            = 0x01,
    FUN_RECV_IND                            = 0x02
};

enum DebugMsg : uint8_t
{
    DEBUG_ECHO_ON                           = 0x01,
    DEBUG_ECHO_OFF                          = 0x02,
    DEBUG_ECHO_REPLY                        = 0x03,
    DEBUG_EPROM_WR_NOTIFY_REQ               = 0x04,
    DEBUG_EPROM_WR_IND                      = 0x05,
    DEBUG_EPROM_WR_NOTIFY_CFM               = 0x06
};

enum SystemMsg : uint8_t
{
    SYS_BATTERY_MEASURE_GET_REQ             = 0x01,
    SYS_BATTERY_MEASURE_GET_RES             = 0x02,
    SYS_RSSI_GET_REQ                        = 0x03,
    SYS_RSSI_GET_RES                        = 0x04,
    SYS_BATTERY_IND_ENABLE_REQ              = 0x05,
    SYS_BATTERY_IND_DISABLE_REQ             = 0x06,
    SYS_BATTERY_IND_LOW_IND                 = 0x07,
    SYS_RESET_REQ                           = 0x08,
    SYS_BATTERY_END_LIFE_IND                = 0x09
};

enum ParameterMsg : uint8_t
{
    PARAM_GET_REQ                           = 0x01,
    PARAM_GET_RES                           = 0x02,
    PARAM_SET_REQ                           = 0x03,
    PARAM_SET_RES                           = 0x04,
    PARAM_GET_DIRECT_REQ                    = 0x05,
    PARAM_GET_DIRECT_RES                    = 0x06,
    PARAM_SET_DIRECT_REQ                    = 0x07,
    PARAM_SET_DIRECT_RES                    = 0x08
};


enum SleepMsg : uint8_t
{
    SLEEP_ENTER_SLEEP_REQ                   = 0x01,
    SLEEP_ENTER_SLEEP_CFM                   = 0x02
};

enum UleVoiceCallMsg : uint8_t
{
    ULE_VOICE_CALL_START_IND                = 0x01,
    ULE_VOICE_CALL_START_RES                = 0x02,
    ULE_VOICE_CALL_END_IND                  = 0x03,
    ULE_VOICE_CALL_END_RES                  = 0x04,
    ULE_VOICE_CALL_ACTIVE_REQ               = 0x05,
    ULE_VOICE_CALL_ACTIVE_RES               = 0x06,
    ULE_VOICE_CALL_CODEC_REQ                = 0x07,
    ULE_VOICE_CALL_CODEC_RES                = 0x08,
    ULE_VOICE_CALL_START_REQ                = 0x09,
    ULE_VOICE_CALL_START_CFM                = 0x0A,
    ULE_VOICE_CALL_END_REQ                  = 0x0B,
    ULE_VOICE_CALL_END_CFM                  = 0x0C,
    ULE_VOICE_CALL_RELEASE_IND              = 0x0D,
    ULE_VOICE_CALL_SET_VOLUME_REQ           = 0x0E,
    ULE_VOICE_CALL_SET_VOLUME_CFM           = 0x0F,
    ULE_VOICE_CALL_VOLUME_UP_REQ            = 0x10,
    ULE_VOICE_CALL_VOLUME_UP_CFM            = 0x11,
    ULE_VOICE_CALL_VOLUME_DOWN_REQ          = 0x12,
    ULE_VOICE_CALL_VOLUME_DOWN_CFM          = 0x13,
    ULE_VOICE_CALL_CONNECTED_IND            = 0x14,
    ULE_VOICE_CALL_STATUS_UPDATE_REQ        = 0x15
};

enum ProductionMsg : uint8_t
{
    PROD_PLACE_HOLDER                       = 0x00,
    PROD_START_REQ                          = 0x01,
    PROD_END_REQ                            = 0x02,
    PROD_CFM                                = 0x03,
    PROD_REF_CLK_TUNE_START_REQ             = 0x04,
    PROD_REF_CLK_TUNE_END_REQ               = 0x05,
    PROD_REF_CLK_TUNE_END_RES               = 0x06,
    PROD_REF_CLK_TUNE_ADJ_REQ               = 0x07,
    PROD_BG_REQ                             = 0x08,
    PROD_BG_RES                             = 0x09,
    PROD_ATE_INIT_REQ                       = 0x0A,
    PROD_ATE_STOP_REQ                       = 0x0B,
    PROD_ATE_CONTINUOUS_START_REQ           = 0x0C,
    PROD_ATE_RX_START_REQ                   = 0x0D,
    PROD_ATE_RX_START_RES                   = 0x0E,
    PROD_ATE_TX_START_REQ                   = 0x0F,
    PROD_ATE_GET_BER_FER_REQ                = 0x10,
    PROD_INIT_EEPROM_DEF_REQ                = 0x11,
    PROD_SPECIFIC_PRESET_REQ                = 0x12,
    PROD_SLEEP_REQ                          = 0x13,
    PROD_SET_SIMPLE_GPIO_LOW                = 0x14,
    PROD_SET_SIMPLE_GPIO_HIGH               = 0x15,
    PROD_GET_SIMPLE_GPIO_STATE              = 0x16,
    PROD_GET_SIMPLE_GPIO_STATE_RES          = 0x17,
    PROD_SET_ULE_GPIO_LOW                   = 0x18,
    PROD_SET_ULE_GPIO_HIGH                  = 0x19,
    PROD_GET_ULE_GPIO_STATE                 = 0x1A,
    PROD_GET_ULE_GPIO_STATE_RES             = 0x1B,
    PROD_SET_ULE_GPIO_DIR_INPUT_REQ         = 0x1C,
    PROD_RESET_EEPROM                       = 0x1D,
    PROD_FW_UPDATE_REQ                      = 0x1E,
    PROD_GPIO_LOOPBACK_TEST_REQ             = 0x1F
};

enum SuotaMsg : uint8_t
{
    SUOTA_NEW_SW_AVAILABLE_IND              = 0x01,
    SUOTA_NEW_SW_RES                        = 0x02,
    SUOTA_DOWNLOAD_START_REQ                = 0x03,
    SUOTA_DOWNLOAD_START_RES                = 0x04,
    SUOTA_IMAGE_READY_IND                   = 0x05,
    SUOTA_IMAGE_READY_RES                   = 0x06,
    SUOTA_UPGRADE_COMPLETED_REQ             = 0x08,
    SUOTA_UPGRADE_COMPLETED_RES             = 0x09,
    SUOTA_READ_FILE_REQ                     = 0x0A,
    SUOTA_READ_FILE_RES                     = 0x0B,
    SUOTA_DOWNLOAD_ABORT_REQ                = 0x0C,
    SUOTA_DOWNLOAD_ABORT_RES                = 0x0D,
    SUOTA_CHECK_FOR_NEW_VERSION_REQ         = 0x0E,
    SUOTA_CHECK_FOR_NEW_VERSION_RES         = 0x0F
};

enum RemoteControlMsg : uint8_t
{
    REMOTE_CONTROL_SEND_CONTROL_REQ         = 0x01
};

enum CertificationMsg : uint8_t
{
    CERT_SET_RESET_SN_REQ                   = 0x01,
    CERT_SET_RESET_SN_RES                   = 0x02,
    CERT_MARK_CCM_KEY_AS_USED_REQ           = 0x03,
    CERT_MARK_CCM_KEY_AS_USED_RES           = 0x04,
    CERT_SET_TX_MIC_CORRUPTION_NUMBER_REQ   = 0x05,
    CERT_SET_TX_MIC_CORRUPTION_NUMBER_RES   = 0x06,
    CERT_SET_TX_SN_REQ                      = 0x07,
    CERT_SET_TX_SN_RES                      = 0x08,
    CERT_GET_TX_SN_REQ                      = 0x09,
    CERT_GET_TX_SN_RES                      = 0x0A,
    CERT_GET_RX_SN_REQ                      = 0x0B,
    CERT_GET_RX_SN_RES                      = 0x0C,
    CERT_SET_TX_ACK_DROP_NUMBER_REQ         = 0x0D,
    CERT_SET_TX_ACK_DROP_NUMBER_RES         = 0x0E,
    CERT_SET_PMID_REQ                       = 0x0F,
    CERT_SET_PMID_RES                       = 0x10,
    CERT_GET_PMID_REQ                       = 0x11,
    CERT_GET_PMID_RES                       = 0x12,
    CERT_SET_PI_REQ                         = 0x13,
    CERT_SET_PI_RES                         = 0x14,
    CERT_GET_PI_REQ                         = 0x15,
    CERT_GET_PI_RES                         = 0x16,
    CERT_SET_PVC_STATE_REQ                  = 0x17,
    CERT_SET_PVC_STATE_RES                  = 0x18
};
} // namespace MSGType


namespace IEType
{
enum IEType : uint8_t
{
	IE_RESPONSE                             = 0x00,
	IE_ATTRIBUTE_ID                         = 0x01,
	IE_ATTRIBUTE_VALUE                      = 0x02,
	IE_UNIT_ADDR                            = 0x03,
	IE_RESPONSE_REQUIRED                    = 0x04,
	IE_FUN                                  = 0x05,
	IE_ALERT                                = 0x06,
	IE_SLEEP_INFO                           = 0x07,
	IE_VERSION                              = 0x09,
	IE_BATTERY_LEVEL                        = 0x0A,
	IE_PARAMETER                            = 0x0B,
	IE_PARAMETER_DIRECT                     = 0x0C,
	IE_GENERAL_STATUS                       = 0x0D,
	IE_DEREGISTRATION                       = 0x0E,
	IE_BIND_ENTRY                           = 0x0F,

	IE_GROUP_ID                             = 0x10,
	IE_GROUP_ENTRY                          = 0x11,
	IE_GROUP_INFO                           = 0x12,
	IE_OTA_COOKIE                           = 0x13,
	IE_ATTR_ADD_REPORT_ENTRY                = 0x15,
	IE_CREATE_ATTR_REPORT_RESPONSE          = 0x16,

	IE_REGISTRATION_RESPONSE                = 0x1C,
	IE_TAMPER_ALERT                         = 0x1D,
	IE_U8                                   = 0x1E,
	IE_BATTERY_MEASURE_INFO                 = 0x1F,

	IE_OTA_CONTROL                          = 0x22,
	IE_IDENTIFY                             = 0x23,
	IE_U32                                  = 0x24,
	IE_BG_REQ                               = 0x25,
	IE_BG_RES                               = 0x26,
	IE_ATE_CONT_REQ                         = 0x27,
	IE_ATE_RX_REQ                           = 0x28,
	IE_ATE_RX_RES                           = 0x29,
	IE_ATE_TX_REQ                           = 0x2A,

	IE_BASE_WANTED                          = 0x2C,
	IE_REPORT_ID                            = 0x2D,
	IE_ADD_REPORT_INFO                      = 0x2E,
	IE_REPORT_INFO                          = 0x2F,

	IE_NEW_SW_INFO                          = 0x32,
	IE_CURRENT_SW_INFO                      = 0x33,
	IE_IMAGE_TYPE                           = 0x34,
	IE_SW_VER_INFO                          = 0x35,
	IE_READ_FILE_DATA_RES                   = 0x36,
	IE_READ_FILE_DATA_REQ                   = 0x37,
	IE_U16                                  = 0x38,
	IE_PMID                                 = 0x39,
	IE_PORTABLE_IDENTITY                    = 0x3A,

	IE_SET_ATTRIBUTE_VALUE                  = 0x3C,
	IE_DEREGISTRATION_RESPONSE              = 0x3D,
	IE_GPIO_STATE                           = 0x3E,
	IE_LINK_MAINTAIN                        = 0x3F,
	IE_ULE_CALL_SETTING                     = 0x40,

	IE_CHECK_SW_VER_RES                     = 0x42,
	IE_BROADCAST_CHANNEL_ALLOCATION         = 0x43,
	IE_BROADCAST_GROUP_TABLE_OPERATION      = 0x44,
	IE_BROADCAST_GROUP_TABLE_RANGE          = 0x45,
	IE_BROADCAST_GROUP_TABLE_ENTRIES        = 0x46
};
} // namespace IE


namespace FUNType
{
enum FUNType : uint8_t
{
    FUN_MSG_COMMAND                        = (0x01),       // A command id for with reference to selected interface
    FUN_MSG_COMMAND_WITH_RESPONSE          = (0x02),       // A command which requires Response from the destination device
    FUN_MSG_COMMAND_RESPONSE               = (0x03),       // A response to the command from destination device
    FUN_MSG_ATTRIBUTE_GET                  = (0x04),       // Get attribute for some interface
    FUN_MSG_ATTRIBUTE_GET_RESPONSE         = (0x05),       // A response for Get Attribute message
    FUN_MSG_ATTRIBUTE_SET                  = (0x06),       // Set an attribute for some interface
    FUN_MSG_ATTRIBUTE_SET_WITH_RESPONSE    = (0x07),       // Set an attribute and request Response for this operation
    FUN_MSG_ATTRIBUTE_SET_RESPONSE         = (0x08),       // A response for Set Attribute or Set attribute with response
    FUN_MSG_GET_ATTRIBUTE_PACK             = (0x09),       // Read one or more parameters of interface in one message
    FUN_MSG_GET_ATTRIBUTE_PACK_RESPONSE    = (0x0A),       // A response to get attribute pack request
    FUN_MSG_SET_ATTRIBUTE_PACK             = (0x0B),       // Set one or more attributes in one message
    FUN_MSG_SET_ATTRIBUTE_PACK_WITH_RESPONSE = (0x0C),     // Set one or more attribute in one message and require response
    FUN_MSG_SET_ATTRIBUTE_PACK_RESPONSE    = (0x0D),       // A response to Set attribute pack command

    FUN_MSG_TYPE_LAST
};
}







/* Order important,... entry for fp_ParamSetHandlers
 Service ID: 0x0203 (PARAMETERS)
 Message ID: 0x01 (PARAM_GET_REQ)
 IE Type: 0x0B (IE_PARAMETER)
 Raw IE : 00.01.00.00 
enum t_en_hanCmndParameter : uint8_t
{
    PARAM_EEPROM_RXTUN                     = 0x0,
    PARAM_EEPROM_IPEI                      = 0x1,
    PARAM_EEPROM_TBR6                      = 0x2,
    PARAM_EEPROM_DECT_CARRIER              = 0x3,
    PARAM_EEPROM_PROD_ENABLE               = 0x4,
    PARAM_EEPROM_EXT_SLOT_TYPE             = 0x5,
    PARAM_EEPROM_FRIENDLY_NAME             = 0x6,
    PARAM_EEPROM_SW_VERISON                = 0x7,
    PARAM_EEPROM_HW_VERISON                = 0x8,
    PARAM_EEPROM_MANUFACTURE_NAME          = 0x9,
    PARAM_EEPROM_INFO_TABLE                = 0xa,
    PARAM_EEPROM_PLUGIN_MAP                = 0xb,
    PARAM_EEPROM_AUX_BG_PROG               = 0xc,
    PARAM_EEPROM_POR_BG_CFG                = 0xd,
    PARAM_EEPROM_DECT_FULL_POWER           = 0xe,
    PARAM_EEPROM_DECT_PA2_COMP             = 0xf,
    PARAM_EEPROM_DECT_SUPPORT_FCC          = 0x10,
    PARAM_EEPROM_DECT_DEVIATION            = 0x11,
    PARAM_EEPROM_HAN_REG_RETRY_TIMEOUT     = 0x12,
    PARAM_EEPROM_HAN_LOCK_MAX_RETRY        = 0x13,
    PARAM_EEPROM_HAN_REG_PIN_CODE          = 0x14,
    PARAM_EEPROM_HAN_ENABLE_AUTO_REG       = 0x15,
    PARAM_EEPROM_HAN_SYS_OFF_USED          = 0x16,
    PARAM_EEPROM_HAN_INFO_LOCATION         = 0x17,
    PARAM_EEPROM_HAN_HBR_OSC               = 0x18,
    PARAM_EEPROM_HAN_RETRANSMIT_URGENT     = 0x19,
    PARAM_EEPROM_HAN_RETRANSMIT_NORMAL     = 0x1a,
    PARAM_EEPROM_HAN_PAGING_CAPS           = 0x1b,
    PARAM_EEPROM_HAN_MIN_SLEEP_TIME        = 0x1c,
    PARAM_EEPROM_HAN_PLUGIN_SUPPORTED      = 0x1d,
    PARAM_EEPROM_DECT_EMC                  = 0x1e,
    PARAM_EEPROM_RSSI_SETTINGS             = 0x1f,
    PARAM_EEPROM_HAN_GENERAL_FLAGS         = 0x20,
    PARAM_EEPROM_HAN_HANDLED_EXTERNALLY    = 0x21,
    PARAM_EEPROM_HAN_ACTUAL_RESPONSE_TIME  = 0x22,
    PARAM_EEPROM_HAN_DEVICE_ENABLE         = 0x23,
    PARAM_EEPROM_HAN_DEVICE_UID            = 0x24,
    PARAM_EEPROM_HAN_SERIAL_NUM            = 0x25,
    PARAM_DEFINED_HF_CORE_RELEASE_VER      = 0x26,
    PARAM_DEFINED_PROFILE_RELEASE_VER      = 0x27,
    PARAM_DEFINED_INTERFACE_RELEASE_VER    = 0x28,
    PARAM_EEPROM_HAN_KEEPALIVE_TIMEOUT     = 0x29,
    PARAM_EEPROM_REGISTRATION_STATUS       = 0x2a,
    PARAM_EEPROM_HAN_HIBERNATION_WATCHDOG  = 0x2b,
    PARAM_EEPROM_ULE_GPIO_MAPPING_EVENT    = 0x2c,
    PARAM_EEPROM_ATTR_REPORTING_SUPPORTED  = 0x2d,

    PARAM_EEPROM_LAST
}*/

/*typedef enum
{
    // Service interfaces
    FUN_INTERFACE_DEVICE_MANAGEMENT         = 0x0001,   // Device Management (Add, View)
    FUN_INTERFACE_BIND_MANAGEMENT           = 0x0002,   // Bind Management (Add, Remove, etc.)
    FUN_INTERFACE_GROUP_MANAGEMENT          = 0x0003,   // Group Management (Add, Remove, View, etc.)
    FUN_INTERFACE_IDENTIFY                  = 0x0004,   // Identify Unit/Group in system
    FUN_INTERFACE_DEVICE_INFORMATION        = 0x0005,   // Device attributes (hw version, sw version, emc, etc.)
    FUN_INTERFACE_ATTRIBUTE_REPORTING       = 0x0006,   // Attributes value reporting
    FUN_INTERFACE_TAMPER_ALERT              = 0x0101,   // When device wants to indicate a Tamper alert
    FUN_INTERFACE_TIME                      = 0x0102,   // Service that allows a device to maintain time referenced to UTC
    FUN_INTERFACE_POWER                     = 0x0110,   // Service that allows a device to provide information about the power supplies it has and their characteristics
    FUN_INTERFACE_RSSI                      = 0x0111,   // Use when device wants to indicate the RSSI
    FUN_INTERFACE_KEEP_ALIVE                = 0x0115,   // When device wants to indicate its alive

    // Functional Interfaces
    FUN_INTERFACE_ALERT                     = 0x0100,   // When device wants to indicate an alert
    FUN_INTERFACE_ON_OFF                    = 0x0200,   // Turning device On, Off
    FUN_INTERFACE_LEVEL_CONTROL             = 0x0201,   // Use to set some device feature to a defined level
    FUN_INTERFACE_COLOR_CONTROL             = 0x0202,   // Use to set a color
    FUN_INTERFACE_SIMPLE_POWER_METERING     = 0x0300,   // Use when device requires doing or providing measurements over electric quantities
    FUN_INTERFACE_TEMPERATURE               = 0x0301,   // Simple Temperature
    FUN_INTERFACE_HUMIDITY                  = 0x0302,   // Simple Humidity
    FUN_INTERFACE_THERMOSTAT                = 0x0303,   // Simple Thermostat
    FUN_INTERFACE_SIMPLE_BUTTON             = 0x0304,   // Use this interface to receive several button press related notifications
    FUN_INTERFACE_SIMPLE_VISUAL_CONTROL     = 0x0305,   // Use this interface to control some visual effects on some visual indicator (display, LED, lamp, etc)
    FUN_INTERFACE_SUOTA                     = 0x0400,   // This service enables a device to perform Software Update over the

    FUN_INTERFACE_RESERVED_OFFSET           = 0x7F00,   // Starting index reserved for proprietary interfaces
}
t_en_hanFunInterfaceId;*/









static constexpr uint16_t MYDEV_ID  = 0x019F;

static const uint8_t GENERAL_HELLO_IND[]               = { 0xDA, 0xDA, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x05, 0xA0, 0x0D, 0x00, 0x05, 0x00, 0x00, 0x00, hi_byte(MYDEV_ID), lo_byte(MYDEV_ID), 0x09, 0x00, 0x05, 0x04, 0x33, 0x2E, 0x30, 0x30 };
static const uint8_t GENERAL_GET_VERSION_RES[]         = { 0xDA, 0xDA, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xCD, 0x09, 0x00, 0x09, 0x08, 0x33, 0x37, 0x2E, 0x31, 0x36, 0x2E, 0x30, 0x38 };
static const uint8_t PARAM_GET_RES_01[]                = { 0xDA, 0xDA, 0x00, 0x16, 0x00, 0x00, 0x02, 0x03, 0x02, 0xFD, 0x0B, 0x00, 0x09, 0x00, 0x01, 0x00, 0x05, 0x03, 0x7C, 0x40, 0x29, 0xDD, 0x00, 0x00, 0x01, 0x00 };
static const uint8_t PARAM_GET_RES_07[]                = { 0xDA, 0xDA, 0x00, 0x1F, 0x00, 0x00, 0x02, 0x03, 0x02, 0x01, 0x0B, 0x00, 0x12, 0x00, 0x07, 0x00, 0x0E, 0x0D, 0x33, 0x37, 0x2E, 0x31, 0x36, 0x2E, 0x30, 0x38, 0x23, 0x31, 0x2E, 0x32, 0x52, 0x00, 0x00, 0x01, 0x00 };
static const uint8_t FUN_MSG_ATTRIBUTE_GET_POSTION[]   = { 0xDA, 0xDA, 0x00, 0x19, 0x00, 0x01, 0x01, 0x08, 0x02, 0xE5, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, hi_byte(MYDEV_ID), lo_byte(MYDEV_ID),  0x01, 0x00, 0x04, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00 };
static const uint8_t FUN_MSG_ATTRIBUTE_GET_OPERATION[] = { 0xDA, 0xDA, 0x00, 0x19, 0x00, 0x00, 0x01, 0x08, 0x02, 0xE7, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, hi_byte(MYDEV_ID), lo_byte(MYDEV_ID),  0x01, 0x00, 0x04, 0x01, 0x02, 0x04, 0x01, 0x00, 0x00 };
static const uint8_t SYS_RSSI_GET_REQ[]                = { 0xDA, 0xDA, 0x00, 0x0A, 0x00, 0x00, 0x02, 0x01, 0x04, 0x51, 0x3D, 0x1E, 0x01, 0x00 }; // orange
static const uint8_t DEV_MGNT_REGISTER_DEVICE_IND[]    = { 0xDA, 0xDA, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x05, 0x51, 0x1C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // green
static const uint8_t DEV_MGNT_DEREGISTER_DEVICE_CFM[]  = { 0xDA, 0xDA, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x07, 0x51, 0x00, 0x00, 0x01, 0x02 }; // CMND_IE_RESPONSE M 0x02 – Not Registered

inline       uint8_t FUN_MSG_COMMAND_POSITION[]        = { 0xDA, 0xDA, 0x00, 0x1A, 0x00, 0x01, 0x01, 0x08, 0x02, 0x32, 0x05, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, hi_byte(MYDEV_ID), lo_byte(MYDEV_ID),  0x01, 0x00, 0x01, 0x01, 0x02, 0x01, 0x01, 0x00, 0x01, 0x4D };
inline       uint8_t FUN_MSG_COMMAND_OPERATION[]       = { 0xDA, 0xDA, 0x00, 0x19, 0x00, 0x01, 0x01, 0x08, 0x02, 0xE6, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, hi_byte(MYDEV_ID), lo_byte(MYDEV_ID),  0x01, 0x00, 0x01, 0x01, 0x02, 0x04, 0x02, 0x00, 0x00};

static constexpr const char *const DDEL = "================================";
static constexpr const char *const SDEL = "--------------------------------";


enum class FrameDir : uint8_t
{
	RX,   // empfangen
	TX    // gesendet
};

enum class DiagState
{
    BOOT,
    NORMAL,
    ERROR
};

inline const char *get_diag_state_name(DiagState state)
{
    switch (state)
    {
        case DiagState::BOOT:   return "BOOT";
        case DiagState::NORMAL: return "NORMAL";
        case DiagState::ERROR:  return "ERROR";
        default: return "UNKNOWN";
    }
}

enum class CoverCommand : uint8_t
{
    OPEN  = 0x01,
    CLOSE = 0x02,
    STOP  = 0x03
};

inline const char *cover_command_name(CoverCommand command)
{
    switch (command)
    {
        case CoverCommand::OPEN:  return "OPEN";
        case CoverCommand::STOP:  return "STOP";
        case CoverCommand::CLOSE: return "CLOSE";
        default:                  return "UNKNOWN";
    }
}

inline const char *get_service_name(uint16_t service)
{
	switch (service)
	{
		// Generic
		case 0x0000: return "GENERAL";
		case 0x0001: return "DEVICE_MANAGEMENT";
		case 0x0004: return "IDENTIFY";
		case 0x0006: return "ATTRIBUTE_REPORTING";
		// Functional Services
		case 0x0100: return "ALERT";
		case 0x0101: return "TAMPER";
		case 0x0102: return "DETECTOR_PROBLEM_ALERT";
		case 0x0103: return "BATTERY";
		case 0x0104: return "KEEP_ALIVE";
		case 0x0105: return "ARM_DISARM";
		case 0x0106: return "ON_OFF";
		case 0x0108: return "FUN";
		case 0x0109: return "DEBUG";
		case 0x010A: return "KEY_PRESS";
		// System Services
		case 0x0201: return "SYSTEM";
		case 0x0202: return "TECHNICIAN";
		case 0x0203: return "PARAMETERS";
		case 0x0204: return "SLEEP";
		case 0x0206: return "MANUFACTURE_CONFIGURATION";
		case 0x020A: return "VOICE_CALL";
		case 0x020B: return "PRODUCTION";
		case 0x020C: return "SUOTA";
		case 0x020D: return "CERTIFICATION";
		case 0x020E: return "REMOTE_CONTROL";
		case 0x020F: return "SUOTA_PROPRIETARY";
		default: return "UNKNOWN";
	}
}

inline const char *get_msg_name(uint16_t service, uint8_t msg)
{
	switch (service)
	{
		case 0x0000: // CMND_SERVICE_ID_GENERAL
			switch (msg)
			{
				case 0x01: return "GENERAL_SET_ATTRIB_REQ";
				case 0x02: return "GENERAL_SET_ATTRIB_RES";
				case 0x03: return "GENERAL_GET_ATTRIB_REQ";
				case 0x04: return "GENERAL_GET_ATTRIB_RES";
				case 0x05: return "GENERAL_HELLO_IND";
				case 0x06: return "GENERAL_ERROR_IND";
				case 0x07: return "GENERAL_LINK_CFM";
				case 0x08: return "GENERAL_GET_STATUS_REQ";
				case 0x09: return "GENERAL_GET_STATUS_RES";
				case 0x0A: return "GENERAL_HELLO_REQ";
				case 0x0B: return "GENERAL_GET_VERSION_REQ";
				case 0x0C: return "GENERAL_GET_VERSION_RES";
				case 0x0D: return "GENERAL_TRANSACTION_START_REQ";
				case 0x0E: return "GENERAL_TRANSACTION_START_CFM";
				case 0x0F: return "GENERAL_TRANSACTION_END_REQ";
				case 0x10: return "GENERAL_TRANSACTION_END_CFM";
				case 0x11: return "GENERAL_LINK_MAINTAIN_START_REQ";
				case 0x12: return "GENERAL_LINK_MAINTAIN_START_CFM";
				case 0x13: return "GENERAL_LINK_MAINTAIN_STOP_REQ";
				case 0x14: return "GENERAL_LINK_MAINTAIN_STOP_CFM";
				case 0x15: return "GENERAL_LINK_MAINTAIN_STOPPED_IND";
				case 0xFE: return "GENERAL_LOG";
				case 0xFF: return "GENERAL_WAKEUP_REQ";
				default:   return "GENERAL_UNKNOWN";
			}

		case 0x0001: // CMND_SERVICE_ID_DEVICE_MANAGEMENT
			switch (msg)
			{
				case 0x01: return "DEV_MGNT_GET_ATTRIB_REQ";
				case 0x02: return "DEV_MGNT_GET_ATTRIB_RES";
				case 0x03: return "DEV_MGNT_REGISTER_DEVICE_REQ";
				case 0x04: return "DEV_MGNT_REGISTER_DEVICE_CFM";
				case 0x05: return "DEV_MGNT_REGISTER_DEVICE_IND";
				case 0x06: return "DEV_MGNT_DEREGISTER_DEVICE_REQ";
				case 0x07: return "DEV_MGNT_DEREGISTER_DEVICE_CFM";
				case 0x08: return "DEV_MGNT_DEREGISTER_DEVICE_IND";
				default:   return "DEV_MGNT_UNKNOWN";
			}

		case 0x0004: // CMND_SERVICE_ID_IDENTIFY
			switch (msg)
			{
				case 0x01: return "IDENTIFY_DEVICE_REQ";
				case 0x02: return "IDENTIFY_DEVICE_RES";
				case 0x03: return "IDENTIFY_SELF_REQ";
				default:   return "IDENTIFY_UNKNOWN";
			}

		case 0x0006: // CMND_SERVICE_ID_ATTRIBUTE_REPORTING
			switch (msg)
			{
				case 0x01: return "ATTRREP_CREATE_PERIODIC_REPORT_REQ";
				case 0x02: return "ATTRREP_CREATE_PERIODIC_REPORT_RES";
				case 0x03: return "ATTRREP_CREATE_EVENT_REPORT_REQ";
				case 0x04: return "ATTRREP_CREATE_EVENT_REPORT_RES";
				case 0x05: return "ATTRREP_ADDENTRY_PERIODIC_REQ";
				case 0x06: return "ATTRREP_ADDENTRY_PERIODIC_RES";
				case 0x07: return "ATTRREP_ADDENTRY_EVENT_REQ";
				case 0x08: return "ATTRREP_ADDENTRY_EVENT_RES";
				case 0x09: return "ATTRREP_DELETE_REPORT_REQ";
				case 0x0A: return "ATTRREP_DELETE_REPORT_RES";
				case 0x0B: return "ATTRREP_PERIODIC_REPORT_NOTIFICATION_IND";
				case 0x0C: return "ATTRREP_EVENT_REPORT_NOTIFICATION_IND";
				case 0x0D: return "ATTRREP_GET_PERIODIC_REPORT_ENTRIES_REQ";
				case 0x0E: return "ATTRREP_GET_PERIODIC_REPORT_ENTRIES_RES";
				case 0x0F: return "ATTRREP_GET_EVENT_REPORT_ENTRIES_REQ";
				case 0x10: return "ATTRREP_GET_EVENT_REPORT_ENTRIES_RES";
				case 0x11: return "ATTRREP_ADD_REPORT_IND";
				case 0x12: return "ATTRREP_ADD_REPORT_RES";
				case 0x13: return "ATTRREP_REPORT_NOTIFICATION_REQ";
				case 0x14: return "ATTRREP_DELETE_REPORT_IND";
				case 0x15: return "ATTRREP_GET_REPORT_VALUES_IND";
				case 0x16: return "ATTRREP_GET_REPORT_VALUES_RES";
				case 0x17: return "ATTRREP_ADD_REPORT_REQ";
				case 0x18: return "ATTRREP_ADD_REPORT_CFM";
				case 0x19: return "ATTRREP_GET_ATTRIB_REQ";
				case 0x1A: return "ATTRREP_GET_ATTRIB_RES";
				case 0x1B: return "ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_REQ";
				case 0x1C: return "ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_RES";
				default:   return "ATTRREP_UNKNOWN";
			}

		case 0x0100: // CMND_SERVICE_ID_ALERT
			switch (msg)
			{
				case 0x1: return "ALERT_GET_ATTRIB_REQ";
				case 0x2: return "ALERT_GET_ATTRIB_RES";
				case 0x3: return "ALERT_NOTIFY_STATUS_REQ";
				case 0x4: return "ALERT_NOTIFY_STATUS_RES";
				case 0x5: return "ALERT_SET_ATTRIB_REQ";
				case 0x6: return "ALERT_SET_ATTRIB_RES";
				case 0x7: return "ALERT_SET_ATTRIB_REQ_WITH_RES";
				case 0x8: return "ALERT_GET_ATTRIB_PACK_REQ";
				case 0x9: return "ALERT_GET_ATTRIB_PACK_RES";
				case 0xA: return "ALERT_GET_ATTRIB_DYN_PACK_REQ";
				case 0xB: return "ALERT_GET_ATTRIB_DYN_PACK_RES";
				case 0xC: return "ALERT_SET_ATTRIB_PACK_REQ";
				case 0xD: return "ALERT_SET_ATTRIB_PACK_RES";
				case 0xE: return "ALERT_SET_ATTRIB_PACK_REQ_WITH_RES";
				case 0xF: return "ALERT_ATOMIC_SET_ATTRIB_PACK_REQ";
				case 0x10: return "ALERT_ATOMIC_SET_ATTRIB_PACK_RES";
				case 0x11: return "ALERT_ATOMIC_SET_ATTRIB_PACK_REQ_WITH_RES";
				default:   return "ALERT_UNKNOWN";
			}

		case 0x0101: // CMND_SERVICE_ID_TAMPER_ALERT
			switch (msg)
			{
				case 0x1: return "TAMPER_ALERT_GET_ATTRIB_REQ";
				case 0x2: return "TAMPER_ALERT_GET_ATTRIB_RES";
				case 0x3: return "TAMPER_ALERT_NOTIFY_STATUS_REQ";
				case 0x4: return "TAMPER_ALERT_NOTIFY_STATUS_RES";
				case 0x5: return "TAMPER_ALERT_GET_ATTRIB_PACK_REQ";
				case 0x6: return "TAMPER_ALERT_GET_ATTRIB_PACK_RES";
				case 0x7: return "TAMPER_ALERT_GET_ATTRIB_DYN_PACK_REQ";
				case 0x8: return "TAMPER_ALERT_GET_ATTRIB_DYN_PACK_RES";
				default:   return "TAMPER_ALERT_UNKNOWN";
			}

		case 0x0104: // CMND_SERVICE_ID_KEEP_ALIVE
			switch (msg)
			{
				case 0x01: return "KEEP_ALIVE_I_AM_ALIVE_REQ";
				case 0x02: return "KEEP_ALIVE_I_AM_ALIVE_RES";
				case 0x03: return "KEEP_ALIVE_GET_ATTRIB_REQ";
				case 0x04: return "KEEP_ALIVE_GET_ATTRIB_RES";
				case 0x05: return "KEEP_ALIVE_SET_ATTRIB_REQ";
				case 0x06: return "KEEP_ALIVE_SET_ATTRIB_RES";
				case 0x07: return "KEEP_ALIVE_SET_ATTRIB_REQ_WITH_RES";
				case 0x08: return "KEEP_ALIVE_I_AM_ALIVE_WITH_RSSI_REQ";
				default:   return "KEEP_ALIVE_UNKNOWN";
			}

		case 0x0106: // CMND_SERVICE_ID_ON_OFF
			switch (msg)
			{
				case 0x01: return "ONOFF_ON_REQ";
				case 0x02: return "ONOFF_ON_RES";
				case 0x03: return "ONOFF_OFF_REQ";
				case 0x04: return "ONOFF_OFF_RES";
				case 0x05: return "ONOFF_TOGGLE_REQ";
				case 0x06: return "ONOFF_TOGGLE_RES";
				case 0x07: return "ONOFF_GET_ATTRIB_REQ";
				case 0x08: return "ONOFF_GET_ATTRIB_RES";
				case 0x09: return "ONOFF_SET_ATTRIB_REQ";
				case 0x0A: return "ONOFF_SET_ATTRIB_RES";
				case 0x0B: return "ONOFF_SET_ATTRIB_REQ_WITH_RES";
				default:   return "ONOFF_UNKNOWN";
			}

		case 0x0108: // CMND_SERVICE_ID_FUN
			switch (msg)
			{
				case 0x1: return "FUN_SEND_REQ";
				case 0x2: return "FUN_RECV_IND";
				default:  return "FUN_UNKNOWN";
			}

		case 0x0109: // CMND_SERVICE_ID_DEBUG
			switch (msg)
			{
				case 0x01: return "DEBUG_ECHO_ON";
				case 0x02: return "DEBUG_ECHO_OFF";
				case 0x03: return "DEBUG_ECHO_REPLY";
				case 0x04: return "DEBUG_EPROM_WR_NOTIFY_REQ";
				case 0x05: return "DEBUG_EPROM_WR_IND";
				case 0x06: return "DEBUG_EPROM_WR_NOTIFY_CFM";
				default:   return "DEBUG_UNKNOWN";
			}

		case 0x0201: // CMND_SERVICE_ID_SYSTEM
			switch (msg)
			{
				case 0x1: return "SYS_BATTERY_MEASURE_GET_REQ";
				case 0x2: return "SYS_BATTERY_MEASURE_GET_RES";
				case 0x3: return "SYS_RSSI_GET_REQ";
				case 0x4: return "SYS_RSSI_GET_RES";
				case 0x5: return "SYS_BATTERY_IND_ENABLE_REQ";
				case 0x6: return "SYS_BATTERY_IND_DISABLE_REQ";
				case 0x7: return "SYS_BATTERY_IND_LOW_IND";
				case 0x8: return "SYS_RESET_REQ";
				case 0x9: return "SYS_BATTERY_END_LIFE_IND";
				default:  return "SYS_UNKNOWN";
			}

		case 0x0203: // CMND_SERVICE_ID_PARAMETERS
			switch (msg)
			{
				case 0x01: return "PARAM_GET_REQ";
				case 0x02: return "PARAM_GET_RES";
				case 0x03: return "PARAM_SET_REQ";
				case 0x04: return "PARAM_SET_RES";
				case 0x05: return "PARAM_GET_DIRECT_REQ";
				case 0x06: return "PARAM_GET_DIRECT_RES";
				case 0x07: return "PARAM_SET_DIRECT_REQ";
				case 0x08: return "PARAM_SET_DIRECT_RES";
				default:   return "PARAM_UNKNOWN";
			}

		case 0x0204: // CMND_SERVICE_ID_SLEEP
			switch (msg)
			{
				case 0x1: return "SLEEP_ENTER_SLEEP_REQ";
				case 0x2: return "SLEEP_ENTER_SLEEP_CFM";
				default:  return "SLEEP_UNKNOWN";
			}

		case 0x020A: // CMND_SERVICE_ID_ULE_VOICE_CALL
			switch (msg)
			{
				case 0x01: return "ULE_VOICE_CALL_START_IND";
				case 0x02: return "ULE_VOICE_CALL_START_RES";
				case 0x03: return "ULE_VOICE_CALL_END_IND";
				case 0x04: return "ULE_VOICE_CALL_END_RES";
				case 0x05: return "ULE_VOICE_CALL_ACTIVE_REQ";
				case 0x06: return "ULE_VOICE_CALL_ACTIVE_RES";
				case 0x07: return "ULE_VOICE_CALL_CODEC_REQ";
				case 0x08: return "ULE_VOICE_CALL_CODEC_RES";
				case 0x09: return "ULE_VOICE_CALL_START_REQ";
				case 0x0A: return "ULE_VOICE_CALL_START_CFM";
				case 0x0B: return "ULE_VOICE_CALL_END_REQ";
				case 0x0C: return "ULE_VOICE_CALL_END_CFM";
				case 0x0D: return "ULE_VOICE_CALL_RELEASE_IND";
				case 0x0E: return "ULE_VOICE_CALL_SET_VOLUME_REQ";
				case 0x0F: return "ULE_VOICE_CALL_SET_VOLUME_CFM";
				case 0x10: return "ULE_VOICE_CALL_VOLUME_UP_REQ";
				case 0x11: return "ULE_VOICE_CALL_VOLUME_UP_CFM";
				case 0x12: return "ULE_VOICE_CALL_VOLUME_DOWN_REQ";
				case 0x13: return "ULE_VOICE_CALL_VOLUME_DOWN_CFM";
				case 0x14: return "ULE_VOICE_CALL_CONNECTED_IND";
				case 0x15: return "ULE_VOICE_CALL_STATUS_UPDATE_REQ";
				default:   return "ULE_VOICE_CALL_UNKNOWN";
			}

		case 0x020B: // CMND_SERVICE_ID_PRODUCTION
			switch (msg)
			{
				case 0x00: return "PROD_PLACE_HOLDER";
				case 0x01: return "PROD_START_REQ";
				case 0x02: return "PROD_END_REQ";
				case 0x03: return "PROD_CFM";
				case 0x04: return "PROD_REF_CLK_TUNE_START_REQ";
				case 0x05: return "PROD_REF_CLK_TUNE_END_REQ";
				case 0x06: return "PROD_REF_CLK_TUNE_END_RES";
				case 0x07: return "PROD_REF_CLK_TUNE_ADJ_REQ";
				case 0x08: return "PROD_BG_REQ";
				case 0x09: return "PROD_BG_RES";
				case 0x0A: return "PROD_ATE_INIT_REQ";
				case 0x0B: return "PROD_ATE_STOP_REQ";
				case 0x0C: return "PROD_ATE_CONTINUOUS_START_REQ";
				case 0x0D: return "PROD_ATE_RX_START_REQ";
				case 0x0E: return "PROD_ATE_RX_START_RES";
				case 0x0F: return "PROD_ATE_TX_START_REQ";
				case 0x10: return "PROD_ATE_GET_BER_FER_REQ";
				case 0x11: return "PROD_INIT_EEPROM_DEF_REQ";
				case 0x12: return "PROD_SPECIFIC_PRESET_REQ";
				case 0x13: return "PROD_SLEEP_REQ";
				case 0x14: return "PROD_SET_SIMPLE_GPIO_LOW";
				case 0x15: return "PROD_SET_SIMPLE_GPIO_HIGH";
				case 0x16: return "PROD_GET_SIMPLE_GPIO_STATE";
				case 0x17: return "PROD_GET_SIMPLE_GPIO_STATE_RES";
				case 0x18: return "PROD_SET_ULE_GPIO_LOW";
				case 0x19: return "PROD_SET_ULE_GPIO_HIGH";
				case 0x1A: return "PROD_GET_ULE_GPIO_STATE";
				case 0x1B: return "PROD_GET_ULE_GPIO_STATE_RES";
				case 0x1C: return "PROD_SET_ULE_GPIO_DIR_INPUT_REQ";
				case 0x1D: return "PROD_RESET_EEPROM";
				case 0x1E: return "PROD_FW_UPDATE_REQ";
				case 0x1F: return "PROD_GPIO_LOOPBACK_TEST_REQ";
				default:   return "PROD_UNKNOWN";
			}

		case 0x020C: // CMND_SERVICE_ID_SUOTA
			switch (msg)
			{
				case 0x1: return "SUOTA_NEW_SW_AVAILABLE_IND";
				case 0x2: return "SUOTA_NEW_SW_RES";
				case 0x3: return "SUOTA_DOWNLOAD_START_REQ";
				case 0x4: return "SUOTA_DOWNLOAD_START_RES";
				case 0x5: return "SUOTA_IMAGE_READY_IND";
				case 0x6: return "SUOTA_IMAGE_READY_RES";
				case 0x8: return "SUOTA_UPGRADE_COMPLETED_REQ";
				case 0x9: return "SUOTA_UPGRADE_COMPLETED_RES";
				case 0xA: return "SUOTA_READ_FILE_REQ";
				case 0xB: return "SUOTA_READ_FILE_RES";
				case 0xC: return "SUOTA_DOWNLOAD_ABORT_REQ";
				case 0xD: return "SUOTA_DOWNLOAD_ABORT_RES";
				case 0xE: return "SUOTA_CHECK_FOR_NEW_VERSION_REQ";
				case 0xF: return "SUOTA_CHECK_FOR_NEW_VERSION_RES";
				default:  return "SUOTA_UNKNOWN";
			}

		case 0x020E: // CMND_SERVICE_ID_REMOTE_CONTROL
			switch (msg)
			{
				case 0x1: return "REMOTE_CONTROL_SEND_CONTROL_REQ";
				default:  return "REMOTE_CONTROL_UNKNOWN";
			}

		case 0x020D: // CMND_SERVICE_ID_CERTIFICATION
			switch (msg)
			{
				case 0x01: return "CERT_SET_RESET_SN_REQ";
				case 0x02: return "CERT_SET_RESET_SN_RES";
				case 0x03: return "CERT_MARK_CCM_KEY_AS_USED_REQ";
				case 0x04: return "CERT_MARK_CCM_KEY_AS_USED_RES";
				case 0x05: return "CERT_SET_TX_MIC_CORRUPTION_NUMBER_REQ";
				case 0x06: return "CERT_SET_TX_MIC_CORRUPTION_NUMBER_RES";
				case 0x07: return "CERT_SET_TX_SN_REQ";
				case 0x08: return "CERT_SET_TX_SN_RES";
				case 0x09: return "CERT_GET_TX_SN_REQ";
				case 0x0A: return "CERT_GET_TX_SN_RES";
				case 0x0B: return "CERT_GET_RX_SN_REQ";
				case 0x0C: return "CERT_GET_RX_SN_RES";
				case 0x0D: return "CERT_SET_TX_ACK_DROP_NUMBER_REQ";
				case 0x0E: return "CERT_SET_TX_ACK_DROP_NUMBER_RES";
				case 0x0F: return "CERT_SET_PMID_REQ";
				case 0x10: return "CERT_SET_PMID_RES";
				case 0x11: return "CERT_GET_PMID_REQ";
				case 0x12: return "CERT_GET_PMID_RES";
				case 0x13: return "CERT_SET_PI_REQ";
				case 0x14: return "CERT_SET_PI_RES";
				case 0x15: return "CERT_GET_PI_REQ";
				case 0x16: return "CERT_GET_PI_RES";
				case 0x17: return "CERT_SET_PVC_STATE_REQ";
				case 0x18: return "CERT_SET_PVC_STATE_RES";
				default:   return "CERT_UNKNOWN";
			}

		default:
			return "UNKNOWN";
	}
}

inline const char *get_ie_name(uint8_t ie)
{
    switch (ie)
    {
        case 0x00: return "IE_RESPONSE";
        case 0x01: return "IE_ATTRIBUTE_ID";
        case 0x02: return "IE_ATTRIBUTE_VALUE";
        case 0x03: return "IE_UNIT_ADDR";
        case 0x04: return "IE_RESPONSE_REQUIRED";
        case 0x05: return "IE_FUN";
        case 0x06: return "IE_ALERT";
        case 0x07: return "IE_SLEEP_INFO";
        case 0x08: return "IE_Reserved";
        case 0x09: return "IE_VERSION";
        case 0x0A: return "IE_BATTERY_LEVEL";
        case 0x0B: return "IE_PARAMETER";
        case 0x0C: return "IE_PARAMETER_DIRECT";
        case 0x0D: return "IE_GENERAL_STATUS";
        case 0x0E: return "IE_DEREGISTRATION";
        case 0x0F: return "IE_BIND_ENTRY";

        case 0x10: return "IE_GROUP_ID";
        case 0x11: return "IE_GROUP_ENTRY";
        case 0x12: return "IE_GROUP_INFO";
        case 0x13: return "IE_OTA_COOKIE";

        case 0x14: return "IE_Reserved";
        case 0x15: return "IE_ATTR_ADD_REPORT_ENTRY";

        case 0x16: return "IE_CREATE_ATTR_REPORT_RESPONSE";

        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
            return "IE_Reserved";

        case 0x1C: return "IE_REGISTRATION_RESPONSE";
        case 0x1D: return "IE_TAMPER_ALERT";
        case 0x1E: return "IE_U8";
        case 0x1F: return "IE_BATTERY_MEASURE_INFO";

        case 0x20:
        case 0x21:
            return "IE_Reserved";

        case 0x22: return "IE_OTA_CONTROL";
        case 0x23: return "IE_IDENTIFY";
        case 0x24: return "IE_U32";
        case 0x25: return "IE_BG_REQ";
        case 0x26: return "IE_BG_RES";
        case 0x27: return "IE_ATE_CONT_REQ";
        case 0x28: return "IE_ATE_RX_REQ";
        case 0x29: return "IE_ATE_RX_RES";
        case 0x2A: return "IE_ATE_TX_REQ";
        case 0x2B: return "Internal";

        case 0x2C: return "IE_BASE_WANTED";
        case 0x2D: return "IE_REPORT_ID";
        case 0x2E: return "IE_ADD_REPORT_INFO";
        case 0x2F: return "IE_REPORT_INFO";

        case 0x30:
        case 0x31:
            return "IE_Reserved";

        case 0x32: return "IE_NEW_SW_INFO";
        case 0x33: return "IE_CURRENT_SW_INFO";
        case 0x34: return "IE_IMAGE_TYPE";
        case 0x35: return "IE_SW_VER_INFO";
        case 0x36: return "IE_READ_FILE_DATA_RES";
        case 0x37: return "IE_READ_FILE_DATA_REQ";
        case 0x38: return "IE_U16";
        case 0x39: return "IE_PMID";
        case 0x3A: return "IE_PORTABLE_IDENTITY";

        case 0x3B:
            return "IE_Reserved";

        case 0x3C: return "IE_SET_ATTRIBUTE_VALUE";
        case 0x3D: return "IE_DEREGISTRATION_RESPONSE";
        case 0x3E: return "IE_GPIO_STATE";
        case 0x3F: return "IE_LINK_MAINTAIN";
        case 0x40: return "IE_ULE_CALL_SETTING";

        case 0x41:
            return "IE_Reserved";

        case 0x42: return "IE_CHECK_SW_VER_RES";
        case 0x43: return "IE_BROADCAST_CHANNEL_ALLOCATION";
        case 0x44: return "IE_BROADCAST_GROUP_TABLE_OPERATION";
        case 0x45: return "IE_BROADCAST_GROUP_TABLE_RANGE";
        case 0x46: return "IE_BROADCAST_GROUP_TABLE_ENTRIES";

        default:
            return "IE_UNKNOWN";
    }
}

inline const char *get_fun_msg_type_name(uint8_t msg_type)
{
    switch (msg_type)
    {
        case 0x01: return "FUN_MSG_COMMAND";
        case 0x02: return "FUN_MSG_COMMAND_WITH_RESPONSE";
        case 0x03: return "FUN_MSG_COMMAND_RESPONSE";
        case 0x04: return "FUN_MSG_ATTRIBUTE_GET";
        case 0x05: return "FUN_MSG_ATTRIBUTE_GET_RESPONSE";
        case 0x06: return "FUN_MSG_ATTRIBUTE_SET";
        case 0x07: return "FUN_MSG_ATTRIBUTE_SET_WITH_RESPONSE";
        case 0x08: return "FUN_MSG_ATTRIBUTE_SET_RESPONSE";
        case 0x09: return "FUN_MSG_GET_ATTRIBUTE_PACK";
        case 0x0A: return "FUN_MSG_GET_ATTRIBUTE_PACK_RESPONSE";
        case 0x0B: return "FUN_MSG_SET_ATTRIBUTE_PACK";
        case 0x0C: return "FUN_MSG_SET_ATTRIBUTE_PACK_WITH_RESPONSE";
        case 0x0D: return "FUN_MSG_SET_ATTRIBUTE_PACK_RESPONSE";
        default:   return "FUN_MSG_UNKNOWN";
    }
}

