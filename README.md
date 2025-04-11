# ADMETCA6424A

ADMETCA6424A is a user-friendly, Arduino-style library for interfacing with the TCA6424A 24-bit I/O expander over I2C. It provides a familiar `digitalRead` / `digitalWrite` interface, plus advanced features such as capacitor-blip based input reading, cached input state tracking, and safe default pin configuration at startup.

## Features

- Arduino-style `DigitalRead(pin)` and `DigitalWrite(pin, value)`
- All pins initialized to `INPUT` mode by default for maximum safety
- Optional capacitor-blip input detection for reliable reads from debounced buttons
- Input caching support for faster loops and consistent multi-pin reads
- Batch I2C access for optimized communication
- Compatible with boards that allow custom I2C remapping (e.g. ESP32)

## What is Capacitor Blip Input Reading?

Many buttons on real-world PCBs are connected to RC debounce networks — often a resistor and a capacitor in parallel with the switch. These circuits stabilize button signals but make simple `digitalRead()` calls unreliable or ambiguous without a pull-up.

This library offers a solution:

1. Temporarily configure the input pin as an output and drive it HIGH. This charges the capacitor.
2. Switch the pin back to input mode and wait briefly.
3. If the capacitor stays charged, the button was not pressed (no path to ground).
4. If the capacitor quickly discharges, it indicates the button is being pressed.

This method lets you **detect button presses even when no pull-up resistors are installed**, and with very high confidence.

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
uint8_t val = TCA.DigitalReadBlip(0);    // cap-blip probing read
TCA.updateInputs();
uint8_t val = TCA.DigitalReadCached(0);  // fast read from cache
```

## Examples

Open the `BasicTest` example to see a complete 4-mode diagnostic cycle:
1. Output pin scanning
2. Standard input reads
3. Capacitor-based reads
4. Cached input reads

This is useful for debugging new hardware or characterizing noisy inputs.

## Installation

Place the `ADMETCA6424A` folder into your `~/Arduino/libraries/` directory, or use the Arduino IDE's "Add .ZIP Library..." feature to install directly.

## License

MIT License
