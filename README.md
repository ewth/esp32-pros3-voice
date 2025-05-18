# ESP32-S3: ProS3 Voice Detection

Using ESP-Skainet on an "Unexpected Maker ProS3" ESP32-S3 development board.

I wanted to play around with machine learning and voice recognition on "the edge" in ESP-Skainet. I only had a ProS3 and an I2S MEMS microphone available.

I am in no way affiliated with Espressif or Unexpected Maker.

## Hardware

* [ProS3 ESP32-S3 Dev board](https://core-electronics.com.au/pros3-esp32-s3-dev-board-1.html).
* [I2S Microphone Module](https://core-electronics.com.au/i2s-microphone-module.html) (incorporates MSM261S4030H0).

## Notes

* Use QSPI PSRAM.
* TODO: Work out directory structure/better way to incorporate into ESP-Skainet.

## Models

So far, the following have been successful:

### VADNet

* Detecting voice activity (as opposed to other sounds/noise).

### Wakenet 9

* Detecting wake up words: "Hey, Wanda", "Alexa", "Hi, Joy".

### MultiNet 7

* Detecting default voice commands.
* Detecting custom voice commands.
