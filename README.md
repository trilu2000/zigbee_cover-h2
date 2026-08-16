# ESP32-H2 Rollotron Zigbee

This project replaces the DECT radio communication of a Rollotron with **Zigbee**, using an **ESP32-H2**.

## Motivation

The original Rollotron uses a DECT chip for wireless communication, while a separate CPU handles the actual cover functions.

The main motivation for this project was the lack of **proactive status updates** from the original DECT integration.

For example, when running an automation, it was not possible to reliably know whether the Rollotron was currently:

- opening
- closing
- idle

Instead, the automation had to actively query the device state.

The goal of this project is therefore not only to control the Rollotron, but also to provide **proactive status information** to Home Assistant.

## Hardware Architecture

The Rollotron consists of two relevant components:

```text
┌──────────────────────┐
│    Rollotron CPU     │
│                      │
│  Cover functions     │
│  Position            │
│  Movement            │
│  Operation state     │
└──────────┬───────────┘
           │
           │ Serial
           │ protocol
           │
┌──────────▼───────────┐
│      DECT Chip       │
│                      │
│  Wireless            │
│  communication       │
└──────────────────────┘

```

The Rollotron CPU and the DECT chip communicate using a **proprietary serial protocol.

For this project, this protocol was analyzed and reproduced using an **ESP32-H2.

The original DECT chip is therefore replaced by the ESP32-H2:


```text
Original:

Rollotron CPU ── Serial protocol ── DECT chip
                                      │
                                     DECT
                                      │
                                      ▼
								   Fritzbox
                                      │
                                      ▼
                                Home Assistant
							(with custom integration)


This project:

Rollotron CPU ── Serial protocol ── ESP32-H2
                                      │
                                    Zigbee
                                      │
                                      ▼
                                Zigbee2MQTT
                                      │
                                      ▼
                                Home Assistant
```


## ESP32-H2

The ESP32-H2 has two main responsibilities:

1. Communicate with the existing Rollotron CPU using the original serial protocol.
2. Provide wireless communication using Zigbee.

The existing Rollotron CPU continues to handle the actual cover functions. The ESP32-H2 essentially replaces the communication interface previously provided by the DECT chip.

## Cover Status

A key part of the implementation is reporting the actual operating state of the Rollotron.

The Rollotron provides information about whether the cover is currently:

- **OPENING**
- **CLOSING**
- **IDLE**

This information is transferred via Zigbee to Home Assistant.

As a result, Home Assistant can use the current cover state directly without requiring an automation to actively poll the device first.

## Position

The Rollotron uses its own internal position representation, which is converted to the percentage-based position used by Zigbee and Home Assistant.

This allows the cover position to be controlled and reported using the standard Home Assistant cover interface.

## Zigbee

The integrated IEEE 802.15.4 radio of the **ESP32-H2** is used for Zigbee communication.

The Zigbee implementation is designed to work with:

- **Zigbee2MQTT**
- **Home Assistant**

The implementation supports, among other things:

- Open
- Close
- Stop
- Set position
- Current position
- Current movement state

## Project Goal

The project follows a simple approach:

> **Keep the original Rollotron cover CPU and replace only the DECT communication with an ESP32-H2 providing Zigbee connectivity.**

This preserves the existing Rollotron cover functionality while providing a modern Zigbee integration with proactive status information for Home Assistant.

## Disclaimer

This is a reverse-engineering project. The serial protocol used between the Rollotron CPU and the original DECT chip was analyzed and reproduced for this implementation.

Use the project at your own risk.