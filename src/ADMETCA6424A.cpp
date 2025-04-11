#include "ADMETCA6424A.h"

ADMETCA6424A::ADMETCA6424A(uint8_t address, uint8_t sda, uint8_t scl)
  : _address(address), _sda(sda), _scl(scl) {
  for (int i = 0; i < 3; i++) {
    inputCache[i] = 0xFF;
    outputState[i] = 0x00;
    directionMask[i] = 0xFF;
  }
}

void ADMETCA6424A::begin() {
  Wire.begin(_sda, _scl);
  delay(100);
  if (isPresent()) {
    for (uint8_t bank = 0; bank < 3; bank++) setDirection(bank, 0xFF);
    
  } else {
    
  }
}

bool ADMETCA6424A::isPresent() {
  Wire.beginTransmission(_address);
  return Wire.endTransmission() == 0;
}

void ADMETCA6424A::DigitalWrite(uint8_t pin, uint8_t value) {
  uint8_t bank = pin / 8, bit = pin % 8;
  directionMask[bank] &= ~(1 << bit);
  setDirection(bank, directionMask[bank]);
  if (value) outputState[bank] |= (1 << bit);
  else       outputState[bank] &= ~(1 << bit);
  writeOutput(bank, outputState[bank]);
}

uint8_t ADMETCA6424A::DigitalRead(uint8_t pin) {
  uint8_t bank = pin / 8, bit = pin % 8;
  return (readInput(bank) >> bit) & 0x01;
}

uint8_t ADMETCA6424A::DigitalReadCached(uint8_t pin) {
  uint8_t bank = pin / 8, bit = pin % 8;
  return (inputCache[bank] >> bit) & 0x01;
}

uint8_t ADMETCA6424A::DigitalReadBlip(uint8_t pin) {
  uint8_t bank = pin / 8, bit = pin % 8;
  writeOutput(bank, outputState[bank] | (1 << bit));
  setDirection(bank, directionMask[bank] & ~(1 << bit));
  delay(CHARGE_TIME_MS);
  setDirection(bank, directionMask[bank] | (1 << bit));
  delay(CHECK_DELAY_MS);
  return (readInput(bank) >> bit) & 0x01;
}

void ADMETCA6424A::updateInputs() {
  for (uint8_t bank = 0; bank < 3; bank++) {
    inputCache[bank] = readInput(bank);
  }
}

void ADMETCA6424A::setAllPinsInput() {
  for (uint8_t bank = 0; bank < 3; bank++) {
    setDirection(bank, 0xFF);
  }
}

void ADMETCA6424A::setDirection(uint8_t bank, uint8_t value) {
  Wire.beginTransmission(_address);
  Wire.write(0x0C + bank);
  Wire.write(value);
  Wire.endTransmission();
  directionMask[bank] = value;
}

void ADMETCA6424A::writeOutput(uint8_t bank, uint8_t value) {
  Wire.beginTransmission(_address);
  Wire.write(0x04 + bank);
  Wire.write(value);
  Wire.endTransmission();
  outputState[bank] = value;
}

uint8_t ADMETCA6424A::readInput(uint8_t bank) {
  Wire.beginTransmission(_address);
  Wire.write(bank);
  if (Wire.endTransmission(false) != 0) return 0xFF;
  Wire.requestFrom(_address, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0xFF;
}
