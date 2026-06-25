# ESP32 IoT Cybersecurity Honeypot with Arduino Uno Alert System

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-Dev%20Module-red?style=for-the-badge&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-Uno-00979D?style=for-the-badge&logo=arduino)
![Security](https://img.shields.io/badge/Cybersecurity-Honeypot-orange?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%7C%20Arduino-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**A low-interaction IoT honeypot that simulates a vulnerable camera device, logs attack behavior, scores threat levels, and triggers physical hardware alerts.**

[Features](#features) · [Hardware](#hardware) · [Setup](#setup) · [Usage](#usage) · [Dashboard](#dashboard) · [Architecture](#architecture)

</div>

---

## Overview

This final-year CSE project demonstrates core cybersecurity concepts through a fully functional IoT honeypot built on embedded hardware:

- **ESP32** creates a Wi-Fi access point named `SmartCamera-IoT` and hosts a convincing fake IP camera web interface
- **Threat Engine** logs every visitor, tracks per-IP behavior, and calculates a threat score
- **Arduino Uno** receives threat level alerts via UART serial and activates LEDs and buzzer accordingly
- **Web Dashboard** at `/dashboard` shows live attack logs with 5-second auto-refresh

> **Educational Use Only.** Deploy on networks you own or have explicit written authorization to monitor.

---

## Features

### Honeypot Engine (ESP32)
- ☁️ Open Wi-Fi AP: `SmartCamera-IoT` (captive portal redirects all DNS)
- 🎭 5 convincing fake IoT pages: `/`, `/login`, `/admin`, `/camera`, `/settings`
- 📋 Logs: IP address, URL, user-agent, timestamp, request count
- 🧮 Threat scoring based on URL sensitivity + behavioral patterns
- 🔄 Circular log buffer (50 entries) + IP tracker (30 unique IPs)
- 📡 Serial2 alert dispatch to Arduino (throttled, 3s minimum interval)
- 🔐 Login credential capture (logged to Serial Monitor)

### Physical Alert System (Arduino Uno)
- 🟢 Green LED → LOW threat
- 🟡 Yellow LED + 1 beep → MEDIUM threat
- 🔴 Red LED + 2 beeps → HIGH threat
- 🚨 Red LED blink + alarm → CRITICAL threat
- ✅ Boot self-test sequence on startup

### Dashboard
- 📊 Real-time stats: total visitors, active connections, threat level, tracked IPs
- 📜 Full attack log table with IP, URL, time, score, level, user-agent
- 🔁 JavaScript auto-refresh every 5 seconds
- 🌐 REST API: `/api/logs` and `/api/stats` return JSON

---

## Hardware

| Component         | Qty | Notes                             |
|-------------------|-----|-----------------------------------|
| ESP32 Dev Module  | 1   | Any 38-pin ESP32                  |
| Arduino Uno       | 1   | Rev3 or compatible                |
| Green LED         | 1   | 5mm, any brand                    |
| Yellow LED        | 1   | 5mm, any brand                    |
| Red LED           | 1   | 5mm, any brand                    |
| Active Buzzer     | 1   | 5V active (not passive)           |
| 220Ω Resistors    | 3   | For LEDs                          |
| 1kΩ Resistor      | 1   | Voltage divider                   |
| 2kΩ Resistor      | 1   | Voltage divider                   |
| Breadboard        | 1   | Half-size or full                 |
| Jumper Wires      | ~20 | Male-to-male                      |
| USB Cables        | 2   | Micro-USB (ESP32) + USB-B (Uno)   |

**Estimated Cost: ₹400–600 / $5–8 USD**

---

## Setup

### 1. Install Arduino IDE & ESP32 Support
```
File → Preferences → Additional Board URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Tools → Board Manager → Search "esp32" → Install "esp32 by Espressif"
```

### 2. Clone This Repository
```bash
git clone https://github.com/Debasish-Nayak-656/ESP32-IoT-Honeypot.git
cd ESP32-IoT-Honeypot
```

### 3. Upload Arduino Code
```
Open: arduino_code/arduino_alert.ino
Board: Arduino Uno
Port:  COMx / /dev/ttyACMx
Upload ✓
```

### 4. Upload ESP32 Code
```
Open: esp32_code/esp32_honeypot.ino
Board: ESP32 Dev Module
Port:  COMx / /dev/ttyUSBx
Upload Speed: 921600
Upload ✓
```

### 5. Wire Hardware

**Voltage divider (3.3V → 2.2V for UART):**
```
ESP32 GPIO17 ──[1kΩ]──┬── Arduino Pin 0 (RX)
                       [2kΩ]
                        GND (shared)
```

**LEDs and buzzer:**
```
Pin 2 ──[220Ω]── Green  LED ── GND
Pin 3 ──[220Ω]── Yellow LED ── GND
Pin 4 ──[220Ω]── Red    LED ── GND
Pin 8 ──────────── Buzzer  ── GND
```

---

## Usage

1. Power both boards (USB)
2. On any phone or laptop, scan Wi-Fi and connect to **SmartCamera-IoT** (no password)
3. Open browser → `192.168.4.1`
4. Browse the fake camera interface: Home, Login, Admin, Camera, Settings
5. Watch LEDs change as threat score accumulates
6. Open `192.168.4.1/dashboard` to see the analyst view

### Threat Score Progression
```
Visit /          → +10 pts  → LOW    → 🟢 Green LED
Visit /login     → +20 pts  → LOW
Visit /admin     → +30 pts  → MEDIUM → 🟡 Yellow LED
Visit /camera    → +15 pts  → HIGH   → 🔴 Red LED
Visit /settings  → +15 pts
3+ pages visited → +50 pts  → CRITICAL → 🚨 Blink + Alarm
```

---

## Dashboard

Access the analyst dashboard at: `http://192.168.4.1/dashboard`

**Stats panel:** Total visitors · Active connections · Threat level · Tracked IPs  
**Attack log table:** IP Address · URL · Timestamp · Request count · Threat score · Level · User-Agent  
**REST API:**
- `GET /api/logs` → JSON array of all log entries
- `GET /api/stats` → JSON object with current statistics

---

## Architecture

```
Attacker Device
      │ Wi-Fi (SmartCamera-IoT)
      ▼
ESP32 Dev Module
  ├── DNS Server (captive portal)
  ├── Web Server :80
  │    ├── /           Fake IoT Home
  │    ├── /login      Fake Login
  │    ├── /admin      Fake Admin Panel
  │    ├── /camera     Fake Live Feed
  │    ├── /settings   Fake Settings
  │    ├── /dashboard  Analyst Dashboard
  │    └── /api/*      JSON APIs
  ├── Threat Engine
  │    ├── Per-IP behavior tracking
  │    ├── URL-based scoring
  │    └── Threat level classification
  └── Serial2 (GPIO17) → UART
            │
      Arduino Uno
        ├── Green  LED (Pin 2) → LOW
        ├── Yellow LED (Pin 3) → MEDIUM
        ├── Red    LED (Pin 4) → HIGH
        └── Buzzer    (Pin 8) → CRITICAL
```

---

## Repository Structure

```
ESP32-IoT-Honeypot/
├── esp32_code/
│   ├── esp32_honeypot.ino      Main ESP32 sketch
│   └── honeypot_pages.h        HTML page builders
├── arduino_code/
│   └── arduino_alert.ino       Arduino Uno sketch
├── docs/
│   └── PROJECT_DOCUMENTATION.md  Complete documentation
├── diagrams/
│   └── wiring_diagram.md       Wiring reference
└── README.md
```

---

## Skills Demonstrated

`C++ / Arduino` · `ESP32 Wi-Fi AP` · `HTTP Web Server` · `DNS Captive Portal` · `UART Serial Communication` · `Threat Detection Logic` · `REST API Design` · `Real-Time Dashboard` · `IoT Security` · `Honeypot Architecture`

---

## License

MIT License — free for educational and personal use.

---

<div align="center">

**Built by Debasish Nayak**  
CSE Final Year · Cuttack, Odisha, India  
[GitHub](https://github.com/Debasish-Nayak-656) · Targeting: SOC Analyst · Security Analyst · Detection Engineering

</div>
