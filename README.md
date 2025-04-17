# LPG Gas Leakage Detection System with GSM Alert and OLED Display

## Overview

This project implements a real-time **LPG Gas Leakage Detection System** using an **Arduino Uno**, an **MQ gas sensor**, and a **SIM900A GSM module** to ensure early detection and instant notification of potential gas leaks. The system is enhanced with a **buzzer**, **servo motor** to simulate gas valve control, **OLED display** for live feedback, and an SMS-based alert mechanism for immediate user notification.

---

## Components Used

| Component            | Description                                                |
|---------------------|------------------------------------------------------------|
| Arduino Uno         | Microcontroller for processing and control                |
| MQ Gas Sensor       | Detects LPG concentration in the air                     |
| SIM900A GSM Module  | Sends SMS alert when gas exceeds threshold               |
| OLED Display (128x64)| Displays gas level and alert status                     |
| Buzzer              | Provides audible alert                                    |
| Red LED             | Visual alert indicator                                    |
| SG90 Servo Motor    | Simulates gas valve shut-off on detection                |
| Breadboard + Wires  | For circuit connections                                   |

---

## Libraries Used

| Library Name               | Functionality                                                                 |
|----------------------------|------------------------------------------------------------------------------|
| `Servo.h`                 | Controls the SG90 servo motor for simulating valve open/close positions       |
| `SoftwareSerial.h`        | Enables serial communication with the SIM900A GSM module using digital pins   |
| `Wire.h`                  | Supports I2C communication used for OLED display                             |
| `Adafruit_GFX.h`          | Graphics core library for drawing text and shapes on the OLED display         |
| `Adafruit_SSD1306.h`      | Driver library for controlling SSD1306-based 128x64 OLED displays              |

---

## How the System Works

1. The **MQ gas sensor** continuously reads gas concentration via analog input.
2. The **OLED display** shows the real-time gas level and system status.
3. If the gas level exceeds the defined threshold:
   - **LED and buzzer** are activated for audible/visual alert.
   - The **servo motor rotates 90°** to simulate closing a gas valve.
   - An **SMS is sent** to a predefined number via the SIM900A GSM module.
4. Once the gas level returns to a safe range:
   - Alerts are cleared.
   - Servo resets to 0° (valve open).
   - The system re-enters monitoring mode.

---

## Features

- Real-time gas level monitoring
- Audible and visual alerts
- Automatic servo actuation for simulated valve shut-off
- SMS alert with gas level data to predefined number
- OLED display for clear feedback
- Configurable threshold and phone number in code

---

## Code Explanation

### Initialization
- The OLED display is initialized **before GSM** to avoid I2C conflicts.
- All peripherals (servo, pins) are configured in `setup()`.

### Main Loop (`loop()`):
- Reads analog value from MQ sensor (`analogRead`).
- Updates OLED with gas level and current status.
- If gas level exceeds threshold:
  - Triggers buzzer, LED, servo, and SMS (only once per alert cycle).
- If gas level is safe:
  - Resets alert status.

### `sendSMS()` Function:
- Sends AT commands to GSM module to switch to text mode.
- Sends alert message including current gas level.
- Uses `gsm.write((char)26)` to signal end of message (Ctrl+Z).

---

## Configuration

- **Threshold**: `gasThreshold = 35;` — can be adjusted for sensitivity.
- **Phone Number**: Hardcoded in `phoneNumber` variable. Change to your own.
- **OLED I2C Address**: Usually `0x3C`; confirmed via I2C scanner sketch.

---

## Notes and Best Practices

- Use a stable **5V 2A** power supply for GSM module to ensure SMS works.
- Ensure **common GND** between Arduino and GSM module.
- If OLED init fails, verify wiring and I2C address (run I2C scanner).
- SIM card must have SMS plan and be inserted properly.
- Ensure that GSM initialization occurs after OLED to avoid I2C lockups.

---

## Future Improvements

- Store phone number in EEPROM for persistence after reset.
- Add automatic logging to SD card or external server.
- Include real-time clock module for timestamping alerts.
- Add Bluetooth or WiFi support for remote control.

---

## File Structure

```
LPG_Gas_Detector/
├── LPG_Gas_Detection.ino         // Main Arduino source file
├── README.md                     // This documentation file
```

---

## Summary

This system provides a robust embedded solution for detecting LPG gas leaks in real-time, providing both local and remote alerts. It is ideal for home or small industrial safety setups and is a practical example of how embedded systems and communication modules can enhance real-world safety applications.

---

Feel free to contribute, fork, or customize for your specific needs.

