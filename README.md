# What is this?

TODO Desc. the rules of häfv.

# Why is this?

TODO

# How to build boes

TODO

## Hardware prerequisites

- Raspberry Pi Pico W
- 64x32 RGB Matrix (HUB75 RGB LED)
- 2x220kOhm resistors

<details>
<summary>Pico RGB matrix pinout</summary>
<br>
TODO add pinout

</details>

## Software prerequisites

The folloing tools and libraries are required to compile and flash the pico.
- Arduino CLI tool (or Arduino IDE, whatever works)
- [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico)
- Adafruit and Arduino libraries (see below)

## Linux guide

Install Pico Arduino core (if not already prepared):
```
# Initialize arduino-cli
arduino-cli config init

# Add pico url to additional_urls like this:
additional_urls: [https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json]

# Update index to download rp2040 config
arduino-cli core update-index

# Download Pico cores
arduino-cli core install rp2040:rp204
```

Install the required libraries:
```
# Note: 'arduino-cli lib install <library>' installs <library>
# and all its dependencies.

# Adafruit library required to driver the led matrix.
arduino-cli lib install "Adafruit Protomatter"

# Protothread / simulated threads in software to schedule
# timer, display and button controllers.
arduino-cli lib install "ArduinoThread"
```

## Compiling and uploading binary

After the prerequisites are satisfied you should be able to compile and upload the binary to the pico.
```
# Compile and upload binary.
# (Note: For me <serial> was set to /dev/ttyACM0)
arduino-cli compile -u -p <serial> -b rp2040:rp2040:rpipicow display/display.ino
```

# Future improvements

- Separate code into thread classes (e.g. display and web server handling in their own respective class)
- Config file handling for constants/delays/logo (USB mass storage) when plugging the cable (also allow uploading configs on web server site?)
- HTML/CSS file handling (USB mass storage).. not the crude http response way that is currently implemented
- CAD PCB to easier replicate/build this project
