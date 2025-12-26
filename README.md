# Temperature and Humidity Alarm System

An ESP32-based IoT monitoring system that tracks environmental conditions and provides real-time alerts. Built with PlatformIO and designed for WPA2 Enterprise network connectivity.

## Overview

This project implements a temperature and humidity monitoring solution using an ESP32 microcontroller. The system connects to enterprise WiFi networks (such as eduroam) and provides environmental data through a companion web interface.

## Features

- Real-time temperature and humidity monitoring
- WPA2 Enterprise (eduroam) network support
- Configurable alarm thresholds
- Web-based user interface (separate repository)

## Project Structure

```
Temp_Hum_Alarm/
├── src/             # Main application source code
├── include/         # Header files
├── lib/             # Project-specific libraries
├── test/            # Unit tests
├── platformio.ini   # PlatformIO configuration
└── .vscode/         # VS Code settings
```

## Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- ESP32 development board
- DHT11/DHT22 temperature and humidity sensor (or compatible)
- Access to a WPA2 Enterprise network (optional)

## Configuration

Create a `config.h` file in the `src/` directory with your network credentials:

```c
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#define EAP_IDENTITY "your_username"
#define EAP_PASSWORD "your_password"
#define SSID "eduroam"

#endif
```

Add `config.h` to your `.gitignore` to prevent credential exposure.

## Building and Flashing

Using PlatformIO CLI:

```cpp
# Build the project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

Using VS Code with PlatformIO extension:

1. Open the project folder in VS Code
2. Click the PlatformIO icon in the sidebar
3. Select "Build" to compile
4. Select "Upload" to flash the ESP32

## Hardware Setup

### Components

- ESP32 development board
- DHT11 or DHT22 sensor
- Buzzer or LED for alarm indication (optional)
- Jumper wires

### Wiring

Connect the DHT sensor to the ESP32:

| DHT Pin | ESP32 Pin |
|---------|-----------|
| VCC     | 3.3V      |
| GND     | GND       |
| DATA    | GPIO (see source) |

## User Interface

The web-based UI for this project is maintained in a separate repository:

https://git.cs.hofstra.edu/h703168431/final_project_ui

## License

MIT License

## Author

[raham24](https://github.com/raham24)
