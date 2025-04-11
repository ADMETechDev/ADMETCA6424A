#ifndef ADMETCA6424A_H
#define ADMETCA6424A_H

#include <Wire.h>

class ADMETCA6424A {
public:
  ADMETCA6424A(uint8_t address = 0x22, uint8_t sda = 4, uint8_t scl = 13);
  void begin();
  bool isPresent();
  void DigitalWrite(uint8_t pin, uint8_t value);
  uint8_t DigitalRead(uint8_t pin);
  uint8_t DigitalReadCached(uint8_t pin);
  uint8_t DigitalReadBlip(uint8_t pin);
  void updateInputs();
  void setAllPinsInput();

private:
  void setDirection(uint8_t bank, uint8_t value);
  void writeOutput(uint8_t bank, uint8_t value);
  uint8_t readInput(uint8_t bank);

  uint8_t _address, _sda, _scl;
  uint8_t inputCache[3];
  uint8_t outputState[3];
  uint8_t directionMask[3];

  static const unsigned long CHARGE_TIME_MS = 1;
  static const unsigned long CHECK_DELAY_MS = 10;
};

#endif
