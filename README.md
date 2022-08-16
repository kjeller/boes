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
- Raspberry Pi Pico Arduino core
- Adafruit and Arduino libraries (see below)

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
arduino-cli compile -p <serial> -b rp2040:rp2040:rpipicow display/display.ino
```

