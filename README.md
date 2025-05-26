# ESP32-S3: ProS3 Voice Detection

Using Espressif's speech recognition on an "Unexpected Maker ProS3" ESP32-S3 development board.

## Overview

I wanted to play around with voice recognition on "the edge". I had a ProS3 and an I2S MEMS microphone available. ESP-Skainet seemed like a good place to start.

I just used a breadboard to make up the "board" used for this project, wiring the I2S microphone to the ESP32-S3.

## Usage

* Wire an I2S microphone module to an ESP32-S3.
* Build and flash the project to the board using ESP-IDF.
* Say "Hey, Wanda".
* Say one of the voice commands (see [Custom Voice Commands](#custom-voice-commands)).

## Hardware

* [ProS3 ESP32-S3 Dev board](https://core-electronics.com.au/pros3-esp32-s3-dev-board-1.html).
* [I2S Microphone Module](https://core-electronics.com.au/i2s-microphone-module.html) (incorporates MSM261S4030H0).

## Software

* [ESP-IDF](https://github.com/espressif/esp-idf)
* [ESP-Skainet](https://github.com/espressif/esp-skainet)
* [ESP-SR](https://github.com/espressif/esp-sr)

## Wiring

> **TODO**: Add a nice wiring diagram.

## BSP & Hardware Driver

ESP-Skainet only supports a handful of specific development boards out of the box.

Taking the `hardware_driver` component from ESP-Skainet, I stripped out the factory boards and created a custom BSP for the project "board".

The result is in `components/ewth_hardware_driver`. I wanted to differentiate it from the base `hardware_driver` to avoid tripping over myself; the naming isn't great but I'll worry about that later.

## Models

I had mixed results with wake word detection. `Alexa` and `Hey, Wanda` worked well; the `Hi, ESP` model wouldn't work at all. Whether it's my Australian accent or some other issue remains undetermined.

The Multinet 7 speech command worked pretty well for voice command detection. I played around with a number of words and phrases, and found it was very good at recognising some, while it struggled with others. This again could be an accent issue.

* Wake word model: `wn9_heywanda_tts` (Wakenet 9)
* Speech command model: `mn7_en` (Multinet 7)

## Custom Voice Commands

Using [g2p-en](https://github.com/Kyubyong/g2p), I converted some graphenes to phonemes as follows:

| Graphene  | Phoneme |
| --------- | ------- |
| `hello`   | `hcLb`  |
| `goodbye` | `GwDBi` |
| `test`    | `TfST`  |
| `ewan`    | `YocN`  |

I had trouble getting it to recognise "hello" so I added a few additional phonemes for it, based on what it was reporting as detected.

These are saved in `data/commands_en.txt`. The root `CMakeLists.txt` file copies this file into the appropriate directory

## Misc Notes

* Haven't optimised anything yet.
* Everything is included in the build. Need to refine.
