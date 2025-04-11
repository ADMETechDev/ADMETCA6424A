/*
  ADMETCA6424A Basic Test

  This example demonstrates the core functionality of the ADMETCA6424A library,
  which wraps around the TCA6424A 24-bit I/O expander to provide an Arduino-style
  digitalRead/digitalWrite interface, safe pin initialization, and advanced
  capacitor-discharge-based input detection.

  Test Routine (runs in loop):
  1. [Test 1] Sequential output sweep across all 24 pins (0–23)
  2. [Test 2] Standard digitalRead (live, unbuffered)
  3. [Test 3] Capacitor blip read (safe for debounced input detection)
  4. [Test 4] Cached read after a single updateInputs() call

  SDA = pin 4, SCL = pin 13 by default, but can be overridden.
*/

// Include the library
#include <ADMETCA6424A.h>

// Default for boards with fixed I2C pins (e.g., Uno, Mega)
ADMETCA6424A TCA;

// For boards like ESP32 or others that allow custom I2C pin mapping, use the following:
// ADMETCA6424A TCA(20, 21);  // SDA = GPIO20, SCL = GPIO21

void setup() {
  Serial.begin(115200);
  delay(500);
  TCA.begin();
  if (TCA.isPresent()) {
    Serial.println("✅ TCA6424A initialized.");
  } else {
    Serial.println("❌ TCA6424A not found.");
  }
}

void loop() {
  // === Test 1: Sequential HIGH sweep ===
  Serial.println("\n[Test 1] Sequential HIGH sweep across all 24 pins");
  for (uint8_t pin = 0; pin < 24; pin++) {
    Serial.print("Setting HIGH on pin ");
    Serial.println(pin);
    for (uint8_t p = 0; p < 24; p++) TCA.DigitalWrite(p, (p == pin) ? HIGH : LOW);
    delay(500);
  }

  // === Test 2: Standard digitalRead ===
  Serial.println("\n[Test 2] Input read without blipping (live read)");
  TCA.setAllPinsInput();
  for (int t = 0; t < 20; t++) {
    Serial.print("[Standard Input] P2: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalRead(16 + b));
    Serial.print(" P1: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalRead(8 + b));
    Serial.print(" P0: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalRead(0 + b));
    Serial.println();
    delay(500);
  }

  // === Test 3: Capacitor-based blip read ===
  Serial.println("\n[Test 3] Capacitor blip input read");
  for (int t = 0; t < 20; t++) {
    Serial.print("[Blip Input]     P2: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadBlip(16 + b));
    Serial.print(" P1: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadBlip(8 + b));
    Serial.print(" P0: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadBlip(0 + b));
    Serial.println();
    delay(500);
  }

  // === Test 4: Cached read ===
  Serial.println("\n[Test 4] Cached input read after updateInputs()");
  TCA.updateInputs();
  for (int t = 0; t < 20; t++) {
    Serial.print("[Cached Input]   P2: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadCached(16 + b));
    Serial.print(" P1: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadCached(8 + b));
    Serial.print(" P0: ");
    for (int b = 7; b >= 0; b--) Serial.print(TCA.DigitalReadCached(0 + b));
    Serial.println();
    delay(500);
  }

  Serial.println("\n[Restarting tests...]");
}
