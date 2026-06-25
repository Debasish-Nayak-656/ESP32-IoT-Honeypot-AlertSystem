# ESP32 IoT Cybersecurity Honeypot — Project Documentation

## Abstract

This project implements a cybersecurity honeypot using an ESP32 microcontroller that simulates a vulnerable IoT camera. The device creates an open Wi-Fi AP named "SmartCamera-IoT," hosts a realistic fake camera web interface, and logs all unauthorized connection attempts. A threat-scoring algorithm classifies attacker behavior into four levels (LOW, MEDIUM, HIGH, CRITICAL). The ESP32 communicates threat levels to an Arduino Uno via UART, which activates LEDs and buzzer alerts. A real-time web dashboard displays all attack logs and statistics. This project demonstrates honeypot design, threat detection logic, microcontroller serial communication, and IoT security monitoring.

---

## 1. Introduction

IoT devices are prime targets for attackers due to default credentials, exposed admin panels, and minimal hardening. A **honeypot** is a deliberately deceptive decoy system that attracts and logs unauthorized access. Since no legitimate user should ever connect to a honeypot, every connection is inherently suspicious — making honeypot logs high-fidelity, low-noise threat intelligence.

This is a **low-interaction honeypot**: it presents a convincing IoT camera web interface but executes no real OS commands. It is designed for educational deployment in isolated lab environments.

---

## 2. Objectives

1. Understand and implement the honeypot security concept on embedded hardware
2. Build a realistic fake IoT device interface to attract unauthorized access
3. Develop a threat-scoring algorithm to classify attacker behavior by reconnaissance patterns
4. Establish UART serial communication between ESP32 and Arduino Uno
5. Create a real-time web dashboard with auto-refreshing attack logs
6. Implement a physical alert system using LEDs and buzzer

---

## 3. Hardware Requirements

| Component         | Qty | Purpose                        |
|-------------------|-----|--------------------------------|
| ESP32 Dev Module  | 1   | Main honeypot + web server     |
| Arduino Uno       | 1   | Physical alert controller      |
| Green LED         | 1   | LOW threat indicator           |
| Yellow LED        | 1   | MEDIUM threat indicator        |
| Red LED           | 1   | HIGH / CRITICAL indicator      |
| Active Buzzer     | 1   | CRITICAL threat alarm          |
| 220Ω Resistors    | 3   | LED current limiting           |
| 1kΩ Resistor      | 1   | Voltage divider (UART)         |
| 2kΩ Resistor      | 1   | Voltage divider (UART)         |
| Breadboard        | 1   | Prototyping                    |
| Jumper Wires      | ~20 | Connections                    |
| USB Cables        | 2   | Power + programming            |

---

## 4. Software Requirements

| Software                  | Purpose                          |
|---------------------------|----------------------------------|
| Arduino IDE 2.x           | Code editor and uploader         |
| ESP32 Board Package        | esp32 by Espressif               |
| Arduino Uno Board Package  | Pre-installed in Arduino IDE     |
| Web Browser               | Accessing honeypot dashboard     |

No external libraries needed — all dependencies are included with the ESP32 board package.

---

## 5. System Architecture

### 5.1 Block Diagram

```
┌──────────────────────────────────────────────────────┐
│                  ATTACKER DEVICE                     │
│           (Phone / Laptop / Port Scanner)            │
└──────────────────────┬───────────────────────────────┘
                       │ Wi-Fi 802.11b/g/n
                       │ Connects to "SmartCamera-IoT"
                       ▼
┌──────────────────────────────────────────────────────┐
│                  ESP32 DEV MODULE                    │
│                                                      │
│  [Wi-Fi AP "SmartCamera-IoT"] [DNS Captive Portal]  │
│              │                                       │
│  [Web Server Port 80]                                │
│   ├── /           → Fake Home Page                  │
│   ├── /login      → Fake Login (captures creds)     │
│   ├── /admin      → Fake Admin Panel                │
│   ├── /camera     → Fake Live Feed                  │
│   ├── /settings   → Fake Settings                   │
│   ├── /dashboard  → Analyst Dashboard               │
│   ├── /api/logs   → JSON Attack Log                 │
│   └── /api/stats  → JSON Statistics                 │
│              │                                       │
│  [Threat Engine]                                     │
│   ├── IP Tracker (per-IP scores + page visits)      │
│   ├── Scoring Algorithm (URL + behavior based)      │
│   └── Level Calculator (LOW/MED/HIGH/CRITICAL)      │
│              │                                       │
│  [Serial2 Alert Dispatcher → GPIO17 TX]             │
└──────────────────────┬───────────────────────────────┘
                       │ UART 9600 baud
                       │ (via voltage divider 3.3V→5V)
                       ▼
┌──────────────────────────────────────────────────────┐
│                  ARDUINO UNO                         │
│  [Serial Command Receiver] → Reads 'L','M','H','C'  │
│  [Alert Controller]                                  │
│   ├── 'L' → Green LED ON                            │
│   ├── 'M' → Yellow LED ON + 1 beep                  │
│   ├── 'H' → Red LED ON + 2 beeps                    │
│   └── 'C' → Red LED BLINK + ascending alarm         │
│         │          │          │          │           │
│   [GREEN LED] [YELLOW LED] [RED LED] [BUZZER]        │
└──────────────────────────────────────────────────────┘
```

### 5.2 Threat Scoring Matrix

| Trigger                             | Points |
|-------------------------------------|--------|
| Homepage visit (/)                  | +10    |
| Login page visit (/login)           | +20    |
| Admin page visit (/admin)           | +30    |
| Camera page visit (/camera)         | +15    |
| Settings page visit (/settings)     | +15    |
| More than 5 total requests          | +30    |
| 3 or more distinct sensitive pages  | +50    |

| Score Range | Threat Level | LED Output  | Buzzer      |
|-------------|-------------|-------------|-------------|
| 0 – 39      | LOW         | Green solid | Silent      |
| 40 – 79     | MEDIUM      | Yellow solid| 1 short beep|
| 80 – 149    | HIGH        | Red solid   | 2 beeps     |
| 150+        | CRITICAL    | Red blink   | Alarm pattern|

### 5.3 Communication Protocol (ESP32 → Arduino)

Single ASCII character sent over UART at 9600 baud, terminated with newline.

| Character | Meaning  |
|-----------|---------|
| `L`       | LOW      |
| `M`       | MEDIUM   |
| `H`       | HIGH     |
| `C`       | CRITICAL |

Alerts are throttled to one per 3 seconds to avoid flooding the Arduino.

---

## 6. Wiring Diagram

### Voltage Divider (UART Level Shifting 3.3V → ~2.2V)

```
ESP32 GPIO17 (TX2) ──[1kΩ]──┬── Arduino Pin 0 (RX)
                              │
                            [2kΩ]
                              │
                             GND (shared)
```

### Arduino LED & Buzzer Connections

```
Arduino Pin 2 ──[220Ω]── Green  LED (+) ── GND
Arduino Pin 3 ──[220Ω]── Yellow LED (+) ── GND
Arduino Pin 4 ──[220Ω]── Red    LED (+) ── GND
Arduino Pin 8 ─────────── Buzzer (+)   ── GND
Arduino GND ────────────── ESP32 GND
```

---

## 7. Implementation Steps

### Step 1: Arduino IDE Setup
1. Download Arduino IDE 2.x from arduino.cc
2. Add ESP32 board URL in File → Preferences → Additional Board URLs:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Install **esp32 by Espressif** via Tools → Board Manager

### Step 2: Project File Structure
Place both ESP32 files in the same folder (Arduino IDE requires this):
```
esp32_code/
├── esp32_honeypot.ino     ← open this file
└── honeypot_pages.h       ← must be in same folder
arduino_code/
└── arduino_alert.ino
```

### Step 3: Upload Arduino Code
1. Connect Arduino Uno via USB
2. Select Board: **Arduino Uno**, correct COM port
3. Upload `arduino_alert.ino`
4. Verify: LEDs cycle green→yellow→red on boot, then green stays on

### Step 4: Upload ESP32 Code
1. Disconnect Arduino
2. Connect ESP32 via USB
3. Select Board: **ESP32 Dev Module**
4. Upload Speed: 921600, Flash Size: 4MB (32Mb)
5. Hold BOOT button during upload if needed
6. Open Serial Monitor @ 115200 baud — verify AP started

### Step 5: Wire Hardware
Follow wiring diagram in Section 6. Build voltage divider on breadboard first.

### Step 6: Test
1. Connect phone to Wi-Fi: **SmartCamera-IoT**
2. Browse to **192.168.4.1**
3. Navigate: Home → Login → Admin → Camera → Settings
4. Open **192.168.4.1/dashboard** to see attack logs
5. Observe LEDs escalate as threat score rises

---

## 8. Testing Procedure

| Test                        | Expected Result                              | Pass? |
|-----------------------------|----------------------------------------------|-------|
| Phone connects to AP        | "SmartCamera-IoT" visible, connects          | [ ]   |
| Homepage loads              | Fake camera device page renders              | [ ]   |
| /login POST                 | Credentials appear in Serial Monitor         | [ ]   |
| Score progression           | LOW→MEDIUM→HIGH→CRITICAL as pages visited    | [ ]   |
| Green LED on LOW            | Lights at boot and low-score states          | [ ]   |
| Yellow LED on MEDIUM        | Changes at score 40+                         | [ ]   |
| Red LED on HIGH             | Changes at score 80+                         | [ ]   |
| Buzzer on CRITICAL          | Sounds alarm pattern at score 150+           | [ ]   |
| Dashboard loads             | All entries visible at /dashboard            | [ ]   |
| /api/logs returns JSON      | Valid JSON array                             | [ ]   |
| 5s auto-refresh             | Dashboard updates without page reload        | [ ]   |

---

## 9. Security Notes & Ethical Use

### What Is a Honeypot?
A honeypot is a cybersecurity defensive tool that creates a decoy system to attract, detect, and study unauthorized access attempts. It provides high-quality, low-noise threat intelligence because any connection to it is inherently suspicious.

### Ethical Deployment Rules
- Deploy **only on networks you own** or have explicit written authorization to monitor
- This project is **educational** — for lab environments, student projects, and CTF challenges
- **Do not deploy on public internet** without legal consultation
- Captured data (including any credential attempts) must be treated as sensitive and deleted after analysis
- In India, unauthorized interception of communications may implicate the IT Act 2000 — always obtain proper authorization

### Limitations of This Project
- **Low-interaction**: captures page visits and login attempts only; does not capture exploit payloads
- **No persistence**: logs lost on power cycle (add SD card for persistence)
- **Fingerprintable**: sophisticated scanners may identify ESP32 HTTP stack
- **Range**: Wi-Fi AP covers ~30–50m radius

---

## 10. Future Scope

1. SD card logging for persistent forensic records
2. OLED display showing live threat level
3. Telegram bot push alerts on CRITICAL
4. Simulated SSH/Telnet listeners on additional ports
5. Export logs in SIEM-compatible formats (CEF, Syslog)
6. Battery + solar power for field deployment

---

*Debasish Nayak | GitHub: Debasish-Nayak-656 | CSE Final Year Project*
