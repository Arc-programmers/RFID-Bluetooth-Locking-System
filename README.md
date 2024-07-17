# RFID-Bluetooth-Locking-System
This project implements an RFID access control system with Bluetooth integration using an ESP32 microcontroller. The system reads RFID cards and checks them against a list of allowed cards. If the card is allowed, it triggers a relay and provides visual and auditory feedback. Additionally, access can be granted via a Bluetooth command.

## Features

- RFID card reading using the MFRC522 module
- Bluetooth connectivity
- Visual feedback via an LED
- Auditory feedback via a buzzer
- Relay control for door access
- Watchdog timer for improved reliability

## Components

- ESP32 microcontroller
- MFRC522 RFID module
- Buzzer
- LED
- Relay

## Wiring

| Component      | Pin       |
| -------------- | --------- |
| MFRC522 SS     | GPIO 5    |
| MFRC522 RST    | GPIO 3    |
| MFRC522 SCK    | GPIO 15   |
| MFRC522 MOSI   | GPIO 19   |
| MFRC522 MISO   | GPIO 18   |
| Buzzer         | GPIO 23   |
| Relay          | GPIO 4    |
| LED            | GPIO 22   |

## Setup

1. Clone this repository.
2. Open the project in the Arduino IDE.
3. Install the required libraries:
    - `MFRC522`
    - `BluetoothSerial`
    - `esp_task_wdt.h` (included with ESP32 core)
4. Upload the code to your ESP32.

## Usage

1. Power the ESP32 and ensure all components are connected correctly.
2. Use an RFID card to test the access control. The card UID will be printed to the serial monitor.
3. Update the `allowedCards` array in the code with the UIDs of the cards you want to allow access.
4. Use a Bluetooth terminal app to connect to the ESP32 and send the `$` command to grant access remotely.
