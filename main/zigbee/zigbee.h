#pragma once

#include <stdint.h>

enum
{
    ZB_OPERATION_IDLE    = 0,
    ZB_OPERATION_OPENING = 1,
    ZB_OPERATION_CLOSING = 2
};

enum class ZigbeeCommand : uint8_t
{
    OPEN,
    CLOSE,
    STOP,
    SET_POSITION
};

struct ZigbeeEvent
{
    ZigbeeCommand command;
    uint8_t position;
};

enum class ZigbeeStatus : uint8_t
{
    START_PAIR,
    HAS_JOINED,
    ALREADY_JOINED,
    PAIR_FAILED,
    Start_DEREGISTER,
    IS_DEREGISTERED
};

struct ZigbeeConfig
{
    const char *manufacturer;
    const char *model;
};




extern "C" void on_zigbee_event(const ZigbeeEvent &event);
extern "C" void on_zigbee_status(const ZigbeeStatus status);

void zigbee_init(const ZigbeeConfig &config);
void zigbee_pair();
void zigbee_factory_reset();
void zigbee_status();
bool zigbee_is_joined();
bool zigbee_is_factory_new();
void zigbee_set_position(uint8_t position);
void zigbee_set_operation(uint8_t operation);
void zigbee_report_attribute(uint16_t attribute_id);

