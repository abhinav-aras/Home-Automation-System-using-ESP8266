# Home Automation System Using ESP8266 with Google Assistant Integration

An IoT-based home automation project that enables remote control of household appliances using **Google Assistant**, an **ESP8266 Wi-Fi microcontroller**, and **Adafruit IO MQTT feeds**. An eight-channel relay board switches the connected appliances. The system also includes an **MCB for circuit protection** and **manual wall switches wired in a two-way switching arrangement** with the relay-controlled loads, allowing appliances to be operated manually as well as through the automation system.

> **Electrical safety:** Mains wiring can cause electric shock, fire, or serious injury. The MCB and relay board must be appropriately rated and installed with suitable wiring, earthing, enclosure, and isolation. An MCB does not replace all required electrical protections. Mains-side installation and testing should be carried out by a qualified electrician. Never work on an energized circuit.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software and Libraries](#software-and-libraries)
- [Relay and GPIO Configuration](#relay-and-gpio-configuration)
- [Adafruit IO and Google Assistant Setup](#adafruit-io-and-google-assistant-setup)
- [Installation and Configuration](#installation-and-configuration)
- [Working Principle](#working-principle)
- [Manual Control and Protection](#manual-control-and-protection)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)

## Project Overview

The system integrates voice-controlled home automation with manual switching. Google Assistant commands are connected to the Adafruit IO feeds through the configured integration or automation service. The ESP8266 connects to Wi-Fi, subscribes to the corresponding MQTT feeds, and controls the eight-channel relay board according to the values received. The relay contacts switch the connected appliances.

Manual switches are also provided in a two-way switching arrangement so that the connected loads can be operated locally. An MCB is included for overcurrent/short-circuit protection as appropriate to the circuit design.

## Features

- Voice-based appliance control using Google Assistant integration.
- Wi-Fi-based control using an ESP8266.
- MQTT communication with Adafruit IO.
- Eight-channel relay board for switching multiple loads.
- Manual control through two-way switch connections.
- MCB included as part of the electrical protection arrangement.
- Serial Monitor messages for Wi-Fi, MQTT, and incoming feed values.

## System Architecture

**Google Assistant → Configured integration/automation service → Adafruit IO feeds → MQTT over Wi-Fi → ESP8266 → 8-channel relay board → Household appliances**

Manual wall switches provide a separate local control path through the project's two-way switching arrangement. The exact mains wiring depends on the circuit design and must be verified by a qualified electrician.

## Hardware Requirements

- ESP8266 development board (for example, NodeMCU)
- Eight-channel relay board compatible with the controller and intended loads
- Household appliances suitable for the relay ratings
- Manual switches wired in the intended two-way switching arrangement
- MCB selected and installed for the circuit's requirements
- Appropriate wiring, terminals, connectors, enclosure, and earthing as required
- USB cable and computer for programming
- Wi-Fi network with internet access

**Component ratings matter:** Confirm relay contact ratings for the actual load type, including inrush current for inductive loads. Confirm that the relay board's input voltage and logic levels are compatible with the ESP8266. Use suitable protection, wiring, clearances, and enclosure for mains voltage.

## Software and Libraries

- Arduino IDE
- ESP8266 board support package for Arduino
- `ESP8266WiFi` library (included with ESP8266 board support)
- **Adafruit MQTT Library** by Adafruit
- Adafruit IO account and feeds
- Google Assistant integration configured through the service or automation platform used by the project

The exact steps for Google Assistant integration depend on the service used to pass voice commands to Adafruit IO. Configure that integration so each appliance command publishes the appropriate value to its matching feed.

## Relay and GPIO Configuration

The supplied ESP8266 sketch uses the following pin assignments:

| Relay / feed | ESP8266 GPIO | Common NodeMCU label |
|---|---:|---|
| Relay 1 (`LED1`) | GPIO5 | D1 |
| Relay 2 (`LED2`) | GPIO4 | D2 |
| Relay 3 (`LED3`) | GPIO0 | D3 |
| Relay 4 (`LED4`) | GPIO1 | TX |
| Relay 5 (`LED5`) | GPIO2 | D4 |
| Relay 6 (`LED6`) | GPIO15 | D8 |
| Relay 7 (`LED7`) | GPIO13 | D7 |
| Relay 8 (`LED8`) | GPIO12 | D6 |

The feed variables are named `Light1` through `Light8` and subscribe to `LED1` through `LED8`. These names can be updated to match the appliance names for easier maintenance.

**GPIO notes:**
- GPIO0, GPIO2, and GPIO15 are boot-strapping pins; the relay board must not force them into invalid levels during startup.
- GPIO1 is the UART TX pin and may interfere with serial logging or uploading. Consider moving that relay input to a suitable unused GPIO if the hardware design permits.
- Verify the pin labels against your exact ESP8266 board.

## Adafruit IO and Google Assistant Setup

1. Sign in to [Adafruit IO](https://io.adafruit.com/).
2. Create eight feeds named `LED1`, `LED2`, `LED3`, `LED4`, `LED5`, `LED6`, `LED7`, and `LED8`, matching the supplied code.
3. Obtain your Adafruit IO username and key from your account settings.
4. Configure the Google Assistant integration or automation service used by your project to publish `1` or `0` to the relevant feed when an appliance voice command is issued.
5. Test the feeds and relay behavior using the low-voltage controller side before testing the complete installation.

Feed names and payloads must match the code and integration configuration. Do not publish your real Wi-Fi password or Adafruit IO key in a public repository.

## Installation and Configuration

### 1. Configure credentials

Replace the placeholders in the sketch with your own credentials:

```cpp
#define WLAN_SSID "Your_WiFi_SSID"
#define WLAN_PASS "Your_WiFi_Password"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Your_Adafruit_IO_Username"
#define AIO_KEY "Your_Adafruit_IO_Key"
```

Keep credentials private. For a public repository, store secrets separately and exclude them from version control.

### 2. Configure the controller

1. Install the ESP8266 board package and Adafruit MQTT library.
2. Check the GPIO-to-relay input mapping.
3. Confirm the relay board's trigger logic (active-high or active-low). The supplied code passes `0` or `1` directly to `digitalWrite()`, so active-low relay boards may behave in the opposite way.
4. Select the correct board and serial port in Arduino IDE.

### 3. Upload and verify

1. Upload the sketch to the ESP8266.
2. Open Serial Monitor at **9600 baud**.
3. Confirm that the ESP8266 connects to Wi-Fi and then to MQTT.
4. Send test values to each Adafruit IO feed and verify the corresponding relay output on the low-voltage side.

Mains-side commissioning must be performed safely by a qualified electrician. Do not touch, adjust, or rewire energized circuits.

## Working Principle

1. **Initialization:** The ESP8266 starts serial communication and configures eight GPIO pins as outputs.
2. **Wi-Fi connection:** It connects to the configured Wi-Fi network.
3. **MQTT setup:** The device subscribes to the eight Adafruit IO feeds.
4. **Voice command:** The configured Google Assistant integration translates a voice command into a feed update.
5. **Message reception:** The ESP8266 receives the MQTT message for the matching feed.
6. **Relay control:** The sketch converts the received payload to an integer and writes the value to the corresponding GPIO.
7. **Appliance switching:** The relay channel changes state and switches its connected load, subject to the relay board's trigger logic and wiring.
8. **Manual operation:** The two-way manual switching arrangement provides local control independently of the voice command path.

## MQTT Feed Values

| Payload | GPIO output |
|---|---|
| `0` | LOW |
| `1` | HIGH |

This describes the GPIO level, not necessarily the relay's physical ON/OFF state. Relay boards may be active-high or active-low, so verify the module's behavior and adjust the code if needed.

## Manual Control and Protection

- **Manual two-way switching:** The manual switches provide local operation in addition to remote control. Ensure the circuit is designed so that manual switching and relay operation interact as intended.
- **MCB:** The MCB is part of the overcurrent/short-circuit protection design. Its rating and breaking capacity must suit the circuit and wiring. It does not by itself guarantee protection against electric shock.
- **Relay board:** Confirm contact ratings, isolation, creepage/clearance, and compatibility with the connected appliance loads.
- **Safe enclosure and wiring:** Use appropriate insulation, terminals, strain relief, earthing, and enclosure. Have mains wiring inspected and tested by a qualified electrician.

## Troubleshooting

| Problem | What to check |
|---|---|
| Wi-Fi does not connect | Verify the SSID, password, signal strength, and network availability. |
| MQTT connection fails | Check the Adafruit IO username/key, server, port, and internet access. |
| Google Assistant command has no effect | Check the integration/automation service and confirm that it publishes to the correct Adafruit IO feed. |
| Wrong relay operates | Verify the GPIO mapping and feed names. |
| Relay behavior is reversed | Check whether the relay input is active-low and invert the logic if required. |
| ESP8266 fails to boot | Check GPIO0, GPIO2, and GPIO15 startup levels and the relay board's input circuitry. |
| Serial logging or uploading is affected | GPIO1 is UART TX in this configuration; consider moving Relay 4 to another suitable pin. |

## Future Improvements

- Replace generic `LED1`-`LED8` feed names with meaningful appliance names.
- Add MQTT reconnection handling that does not permanently block the program.
- Add status reporting to Adafruit IO so the dashboard can show the latest commanded state.
- Use secure MQTT/TLS where supported by the chosen library and configuration.
- Add startup-safe relay states and explicitly handle active-low relay inputs.
- Document the exact Google Assistant integration service and setup steps.
- Review electrical protection, isolation, and enclosure design with a qualified electrician.

## License

Choose a license before publishing the project publicly. The MIT License is one option if you want others to use, modify, and distribute the code with attribution.
