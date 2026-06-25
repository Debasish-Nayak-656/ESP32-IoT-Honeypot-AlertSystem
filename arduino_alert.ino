/*
 * ============================================================
 *  Arduino Uno Alert System
 *  Project: ESP32 IoT Honeypot with Arduino Uno Alert System
 *  Author:  Debasish Nayak  |  GitHub: Debasish-Nayak-656
 *  Board:   Arduino Uno
 *
 *  Receives single-char commands from ESP32 via Serial:
 *    'L' → LOW      : Green LED on
 *    'M' → MEDIUM   : Yellow LED on
 *    'H' → HIGH     : Red LED on
 *    'C' → CRITICAL : Red LED + Buzzer alarm
 * ============================================================
 *  WIRING
 *    Arduino Pin 2  → Green  LED (+) → 220Ω → GND
 *    Arduino Pin 3  → Yellow LED (+) → 220Ω → GND
 *    Arduino Pin 4  → Red    LED (+) → 220Ω → GND
 *    Arduino Pin 8  → Buzzer (+)     →         GND
 *    Arduino RX(0)  ← ESP32 TX2 (GPIO17) via 1kΩ voltage divider
 *    Arduino GND    → ESP32 GND
 *
 *  NOTE: Use a voltage divider on RX line:
 *    ESP32 TX2 → 1kΩ → Arduino RX
 *                     → 2kΩ → GND
 *  This drops 3.3V to ~2.2V (safe for Arduino 5V logic)
 * ============================================================
 */

// ─── Pin Definitions ─────────────────────────────────────────
#define PIN_LED_GREEN   2
#define PIN_LED_YELLOW  3
#define PIN_LED_RED     4
#define PIN_BUZZER      8

// ─── Timing ──────────────────────────────────────────────────
#define BLINK_INTERVAL    200   // ms per blink cycle for CRITICAL
#define BUZZER_BEEP_ON    150   // ms buzzer ON
#define BUZZER_BEEP_OFF   100   // ms buzzer OFF
#define BUZZER_BEEPS      3     // beeps per CRITICAL alert

// ─── State ───────────────────────────────────────────────────
char     currentLevel = 'L';
unsigned long lastBlink = 0;
bool     blinkState = false;

// ════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);   // Communicates with ESP32

  pinMode(PIN_LED_GREEN,  OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED,    OUTPUT);
  pinMode(PIN_BUZZER,     OUTPUT);

  // Boot test sequence (confirms hardware works)
  bootSequence();

  Serial.println("ARD:READY");
  setLevel('L');   // Default: green
}

// ════════════════════════════════════════════════════════════
void loop() {
  // ── Check for new command from ESP32 ─────────────────────
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    // Flush rest of line
    while (Serial.available() > 0) Serial.read();

    if (cmd == 'L' || cmd == 'M' || cmd == 'H' || cmd == 'C') {
      if (cmd != currentLevel) {
        currentLevel = cmd;
        setLevel(cmd);
      }
    }
  }

  // ── CRITICAL mode: blink red LED continuously ─────────────
  if (currentLevel == 'C') {
    unsigned long now = millis();
    if (now - lastBlink >= BLINK_INTERVAL) {
      blinkState = !blinkState;
      digitalWrite(PIN_LED_RED, blinkState ? HIGH : LOW);
      lastBlink = now;
    }
  }
}

// ════════════════════════════════════════════════════════════
//  ALERT ACTIONS
// ════════════════════════════════════════════════════════════

void setLevel(char level) {
  // Turn all off first
  digitalWrite(PIN_LED_GREEN,  LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED,    LOW);
  noTone(PIN_BUZZER);
  blinkState = false;

  switch (level) {
    case 'L':
      digitalWrite(PIN_LED_GREEN, HIGH);
      break;

    case 'M':
      digitalWrite(PIN_LED_YELLOW, HIGH);
      shortBeep(1);
      break;

    case 'H':
      digitalWrite(PIN_LED_RED, HIGH);
      shortBeep(2);
      break;

    case 'C':
      // Blink handled in loop(); trigger alarm beeps
      digitalWrite(PIN_LED_RED, HIGH);
      criticalAlarm();
      break;
  }
}

void shortBeep(int count) {
  for (int i = 0; i < count; i++) {
    tone(PIN_BUZZER, 1000);
    delay(BUZZER_BEEP_ON);
    noTone(PIN_BUZZER);
    if (i < count - 1) delay(BUZZER_BEEP_OFF);
  }
}

void criticalAlarm() {
  // Ascending tone alarm for CRITICAL
  int freqs[] = {800, 1200, 1600};
  for (int rep = 0; rep < BUZZER_BEEPS; rep++) {
    for (int f : freqs) {
      tone(PIN_BUZZER, f);
      delay(120);
    }
  }
  noTone(PIN_BUZZER);
}

void bootSequence() {
  // Light up each LED in sequence, then buzzer blip
  int leds[] = {PIN_LED_GREEN, PIN_LED_YELLOW, PIN_LED_RED};
  for (int led : leds) {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
  }
  tone(PIN_BUZZER, 1200);
  delay(150);
  noTone(PIN_BUZZER);
}
