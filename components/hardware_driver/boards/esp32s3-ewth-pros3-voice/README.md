# ESP32-S3 ProS3: Voice

## Hardware

1. Unintended Makers ProS3 ESP32-S3 dev board.
2. `SEN0327` I2S Microphone module.
3. `CEO7926` microSD module.

## Wiring

> **TODO:** Add wiring diagram. Flesh out explanation.

Note: Module pin -> ProS3 pin

### I2S Microphone Module

1. `V` -> `3v3.1`
2. `G` -> `GND`
3. `WS` -> `IO14`
4. `LR` -> `GND`
5. `CK` -> `IO12`
6. `DA` -> `IO13`

### microSD Card Module

1. `+5.0V` -> N/A
2. `CD` -> N/A (maybe utilise CD later for a pre-mount check?)
3. `+3.3V` -> `3v3.1`
4. `GND` -> `GND`
5. `CS` -> `IO39`
6. `MOSI` -> `IO35`
7. `CLK` -> `IO36`
8. `MISO` -> `IO37`
