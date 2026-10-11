# IoT Home Automation Using ESP8266, Adafruit IO, and MQTT

A Wi-Fi-based IoT project that lets you control **eight LEDs (or suitable low-voltage output modules)** remotely using Adafruit IO feeds. An ESP8266 subscribes to eight MQTT feeds and switches the corresponding GPIO outputs according to the values received.

> **Note:** This code is configured for LEDs as a demonstration. Do not connect mains-powered appliances directly to ESP8266 GPIO pins. Use properly rated relay modules, isolation, enclosures, and safe wiring if adapting it for real appliances.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software and Libraries](#software-and-libraries)
- [Pin Configuration](#pin-configuration)
- [Adafruit IO Setup](#adafruit-io-setup)
- [Installation and Configuration](#installation-and-configuration)
- [How It Works](#how-it-works)
- [MQTT Feed Values](#mqtt-feed-values)
- [Troubleshooting](#troubleshooting)
- [Possible Improvements](#possible-improvements)

## Project Overview

The ESP8266 connects to a Wi-Fi network and then connects to Adafruit IO using MQTT. It subscribes to eight feeds named `LED1` through `LED8`. When a feed publishes a value, the ESP8266 reads that value and sets the associated GPIO output HIGH or LOW.

This demonstrates the basic IoT workflow:

**Control interface → Adafruit IO feed → MQTT → ESP8266 → Output**

## Features

- Remote control of eight individual outputs.
- Wi-Fi connectivity using the ESP8266.
- MQTT communication with Adafruit IO.
- Independent feed for each LED.
- Serial Monitor messages for connection status and received values.

## Hardware Requirements

- ESP8266 development board (for example, NodeMCU)
- Eight LEDs
- Eight current-limiting resistors appropriate for the LEDs
- Breadboard and jumper wires
- USB cable and computer
- Wi-Fi network with internet access

If you are controlling appliances rather than LEDs, use suitable driver circuitry or properly rated relay modules. Confirm voltage, current, isolation, and GPIO compatibility before connecting hardware.

## Software and Libraries

- Arduino IDE
- ESP8266 board support package for Arduino
- `ESP8266WiFi` library (included with ESP8266 board support)
- **Adafruit MQTT Library** by Adafruit

Install the Adafruit MQTT library through **Arduino IDE → Library Manager**. Select an ESP8266 board that matches your hardware before uploading.

## Pin Configuration

The following GPIO assignments are defined in the sketch:

| Output | ESP8266 GPIO | Common NodeMCU label |
|---|---:|---|
| LED1 | GPIO5 | D1 |
| LED2 | GPIO4 | D2 |
| LED3 | GPIO0 | D3 |
| LED4 | GPIO1 | TX |
| LED5 | GPIO2 | D4 |
| LED6 | GPIO15 | D8 |
| LED7 | GPIO13 | D7 |
| LED8 | GPIO12 | D6 |

**Important pin notes:**

- GPIO0, GPIO2, and GPIO15 are boot-strapping pins. External circuits must not force them to the wrong levels during startup.
- GPIO1 is also the UART TX pin, so using it for LED4 can interfere with Serial output and uploading/debugging.
- GPIO pins are not intended to power high-current loads. Use suitable drivers where needed.
- Verify the GPIO mapping for your exact development board.

## Adafruit IO Setup

1. Sign in to [Adafruit IO](https://io.adafruit.com/).
2. Open **Feeds** and create eight feeds with these names:
   - `LED1`
   - `LED2`
   - `LED3`
   - `LED4`
   - `LED5`
   - `LED6`
   - `LED7`
   - `LED8`
3. Find your Adafruit IO username and key in your account settings.
4. Create a dashboard and add a control for each feed. Toggle controls are convenient for sending `0` and `1`.

Feed names must match the names used in the sketch. Adafruit IO feed names are used in the MQTT topics as `username/feeds/LED1`, etc.

## Installation and Configuration

### 1. Configure credentials

In the Arduino sketch, replace the placeholder values with your own credentials:

```cpp
#define WLAN_SSID "Your_WiFi_SSID"
#define WLAN_PASS "Your_WiFi_Password"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Your_Adafruit_IO_Username"
#define AIO_KEY "Your_Adafruit_IO_Key"
```

Keep your Adafruit IO key private. Do not commit real Wi-Fi passwords or API keys to a public repository. Consider using a separate secrets file excluded from version control.

### 2. Check the wiring

Connect each LED to its assigned GPIO through a suitable current-limiting resistor, with the correct polarity and a common ground. Check the board's voltage limits and pin mapping before powering it.

### 3. Upload the sketch

1. Open the project sketch in Arduino IDE.
2. Select the correct ESP8266 board and COM/serial port.
3. Install any missing libraries.
4. Upload the code.
5. Open Serial Monitor at **9600 baud**.

### 4. Test the controls

After the Serial Monitor reports that Wi-Fi and MQTT are connected, open your Adafruit IO dashboard and send `1` or `0` to each LED feed. Check that the matching output changes state.

## How It Works

1. **Initialization:** `setup()` starts serial communication and configures the eight output pins.
2. **Wi-Fi connection:** The ESP8266 connects to the configured wireless network.
3. **MQTT subscriptions:** The sketch subscribes to the eight Adafruit IO feeds.
4. **Connection maintenance:** `MQTT_connect()` checks the MQTT connection and attempts to reconnect if disconnected.
5. **Message reception:** `mqtt.readSubscription(20000)` waits for incoming subscription messages.
6. **Output control:** The code identifies which feed received a message, converts the payload to an integer using `atoi()`, and passes the result to `digitalWrite()` for the corresponding GPIO.

## MQTT Feed Values

Publish numeric values to the feeds:

| Payload | GPIO behavior |
|---|---|
| `0` | LOW |
| `1` | HIGH |

This assumes the output circuit is **active-high**. Some relay modules are active-low, so their ON/OFF behavior may be reversed and the control logic may need to be inverted.

## Troubleshooting

| Problem | What to check |
|---|---|
| Wi-Fi never connects | Verify SSID/password, signal strength, and network compatibility. |
| MQTT connection fails | Check the Adafruit IO username/key, internet access, server, and port. |
| Feed updates do not affect outputs | Confirm the feed names, dashboard configuration, subscription topics, and that payloads are `0` or `1`. |
| ESP8266 fails to boot | Check boot-strapping pins GPIO0, GPIO2, and GPIO15 and disconnect circuits that hold them at incorrect startup levels. |
| Serial output is garbled or LED4 behaves unexpectedly | GPIO1 is UART TX; move LED4 to a safer unused GPIO and update the pin definition. |
| Output behavior is reversed | Check whether the connected module is active-low and invert the logic if necessary. |

## Possible Improvements

- Replace the repeated `if/else` blocks with arrays and a common handler.
- Add Wi-Fi and MQTT reconnection handling without permanently blocking the program.
- Use secure MQTT/TLS where supported and appropriate for the device and library configuration.
- Add dashboard indicators or status reporting.
- Move credentials into a separate, ignored secrets file.
- For real appliances, use properly rated, isolated switching hardware and a safe enclosure.

## License

Choose a license before publishing this project publicly. For example, the MIT License is suitable if you want others to use, modify, and distribute the project with attribution.
