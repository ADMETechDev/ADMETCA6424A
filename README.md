# ADMETCA6424A

ADMETCA6424A is a user-friendly, Arduino-style library for interfacing with the TCA6424A 24-bit I/O expander over I2C. It provides a familiar `digitalRead` / `digitalWrite` interface, plus advanced features such as capacitor-blip based input reading, cached input state tracking, and safe default pin configuration at startup.

## Features

- Arduino-style `DigitalRead(pin)` and `DigitalWrite(pin, value)`
- All pins initialized to `INPUT` mode by default for maximum safety
- Capacitor-blip input reading that acts as a substitute for missing internal pullups
- Input caching support for faster loops and consistent multi-pin reads
- Batch I2C access for optimized communication
- Compatible with boards that allow custom I2C remapping (e.g. ESP32)

## Why Capacitor Blipping?

The TCA6424A does **not have internal pull-up resistors**. That means if you're using buttons or open-collector devices without external pull-ups, `digitalRead()` may return unpredictable results.

This library includes an optional **capacitor-blip detection** feature designed specifically for circuits with hardware debounce (i.e., capacitor + resistor networks). It works by:

1. Configuring the pin as an output and driving it HIGH to charge the debounce capacitor.
2. Switching the pin to input mode.
3. Waiting a short delay to allow for potential discharge.
4. Reading the input value — if the capacitor has discharged, a button press (short to ground) is detected.

This lets you reliably detect button presses **even in the absence of pull-ups**.

Call `DigitalReadBlip(pin)` to use this feature.

## Usage

### Default (for Uno, Mega, etc.)

```cpp
#include <ADMETCA6424A.h>
ADMETCA6424A TCA;
```

### Custom SDA/SCL (e.g. ESP32)

```cpp
#include <ADMETCA6424A.h>
ADMETCA6424A TCA(21, 22);  // SDA = GPIO21, SCL = GPIO22
```

### Input Read Modes

```cpp
uint8_t val = TCA.DigitalRead(0);        // standard I2C read
uint8_t val = TCA.DigitalReadBlip(0);    // capacitor-blip probing read
TCA.updateInputs();
uint8_t val = TCA.DigitalReadCached(0);  // fast read from cache
```

## Examples

Open the `BasicTest` example to see a complete 4-mode diagnostic cycle:
1. Output pin scanning
2. Standard input reads
3. Capacitor-based reads
4. Cached input reads

This is useful for debugging new hardware or characterizing noisy or passively pulled-down inputs.

## Installation

Place the `ADMETCA6424A` folder into your `~/Arduino/libraries/` directory, or use the Arduino IDE's "Add .ZIP Library..." feature to install directly.

## License

MIT License
