# Wiring Diagram Reference

## ESP32 Pin Summary

| ESP32 Pin | Function        | Connects To              |
|-----------|-----------------|--------------------------|
| GPIO17    | TX2 (Serial2)   | Voltage divider → Uno RX |
| GPIO16    | RX2 (Serial2)   | (unused in this project) |
| GND       | Ground          | Arduino GND              |
| 3V3       | 3.3V power      | (do not connect to 5V)   |
| USB       | Power + Upload  | Computer USB             |

## Arduino Uno Pin Summary

| Pin   | Function       | Connects To             |
|-------|----------------|-------------------------|
| 0(RX) | Serial RX      | Voltage divider output  |
| 2     | Digital Output | Green LED (+) via 220Ω  |
| 3     | Digital Output | Yellow LED (+) via 220Ω |
| 4     | Digital Output | Red LED (+) via 220Ω    |
| 8     | Digital Output | Buzzer (+)              |
| GND   | Ground         | LED cathodes, Buzzer(-), ESP32 GND |
| 5V    | 5V power       | (do not connect to ESP32 3V3) |

## Voltage Divider Detail

ESP32 outputs 3.3V on TX. Arduino RX expects TTL-compatible voltage
(anything above ~2V is read as HIGH on Arduino's 5V system).
A simple resistor divider protects the Arduino and ensures signal integrity.

```
ESP32 TX2 (3.3V) ──[1kΩ R1]──┬──[2kΩ R2]── GND
                               │
                            Arduino Pin 0 (RX)
                            (reads ~2.2V = logic HIGH)
```

Formula: V_out = 3.3 × (2kΩ / (1kΩ + 2kΩ)) = 3.3 × 0.667 ≈ 2.2V ✓

## Complete Breadboard Layout (text)

```
BREADBOARD
──────────────────────────────────────────────

POWER RAILS
(+) = 5V from Arduino 5V pin
(-) = GND (shared between Arduino GND and ESP32 GND)

VOLTAGE DIVIDER (rows 1-4)
Row 1: ESP32 GPIO17 ──────────► R1 (1kΩ)
Row 2: R1 other end ──┬───────► R2 (2kΩ) ──► (-)rail
                       └───────► Arduino Pin 0

LEDs (rows 10-16)
Row 10: Arduino Pin 2 ──► R3 (220Ω) ──► Green  LED(+)  ──► (-)rail
Row 12: Arduino Pin 3 ──► R4 (220Ω) ──► Yellow LED(+)  ──► (-)rail
Row 14: Arduino Pin 4 ──► R5 (220Ω) ──► Red    LED(+)  ──► (-)rail

BUZZER (row 18)
Row 18: Arduino Pin 8 ────────────────► Buzzer (+)      ──► (-)rail

GND BRIDGE
ESP32 GND ──────────────────────────────────────────► (-)rail
Arduino GND ────────────────────────────────────────► (-)rail
```
