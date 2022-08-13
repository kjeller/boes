#include <Adafruit_Protomatter.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include "timer.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"  
#include "logo.h"

#ifndef GPIO_START
#define GPIO_START 26
#endif
#ifndef GPIO_STOP
#define GPIO_STOP 27
#endif
#ifndef GPIO_RESET
#define GPIO_RESET 28
#endif

// Mask for GPIOs to use as Start, Stop and Reset buttons
#ifndef GPIO_MASK
#define GPIO_MASK (1 >> GPIO_START | 1 >> GPIO_STOP | 1 >> GPIO_RESET)
#endif

uint8_t rgbPins[]  = {2, 3, 4, 5, 8, 9};
uint8_t addrPins[] = {10, 16, 18, 20, 22};
uint8_t clockPin   = 11;
uint8_t latchPin   = 12;
uint8_t oePin      = 13;

Adafruit_Protomatter matrix(
  64,          // Matrix width in pixels
  6,           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);       // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!

int16_t  textX = matrix.width(), // Current text position (X)
         textY,                  // Current text position (Y)
         textMin,                // Text pos. (X) when scrolled off left edge
         hue = 0;
char     str[50];                // Buffer to hold scrolling message text

Timer timer = {Pause, 0, 0, 0, "000", "000"};

char ssid[] = "TEST PICO";        // your network SSID (name)
char pass[] = "test123";    // your network password (use for WPA, or use as key for WEP)


Thread timerThread = Thread();
Thread displayThread = Thread();
Thread wifiThread = Thread();
StaticThreadController<2> controller (&timerThread, &displayThread);

void setup(void) {

  // TODO implement wifi AP and webserver handling
  //if (cyw43_arch_init()) {
    //    printf("failed to initialise\n");
      //  return;
  //}

  //cyw43_arch_enable_ap_mode(ssid, pass, CYW43_AUTH_WPA2_AES_PSK);

  Serial.begin(9600);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  
  if(status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    Serial.println("Matrix error occurred.. cannot continue");
    while(true);
  }

  matrix.setFont(&FreeSerifBold9pt7b); // Use nice bitmap font
  matrix.setTextColor(0xFFFF);         // White
  matrix.setRotation(2);
  int16_t  x1, y1;
  uint16_t w, h;
  sprintf(str, "%s:%s", timer.sec_counter, timer.ms_counter); // Get initial text to get bounds
  matrix.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
  textX = matrix.width() / 2 - (x1 + w / 2); // Center text horizontally
  textY = matrix.height() / 2 - (y1 + h / 2); // Center text vertically

  pinMode(GPIO_START, INPUT_PULLUP);
  pinMode(GPIO_STOP, INPUT_PULLUP);
  pinMode(GPIO_RESET, INPUT_PULLUP);

  timerThread.onRun(updateTimer);
  displayThread.onRun(updateDisplay);
  wifiThread.onRun(updateWifi);
  timerThread.setInterval(1); // update every 1ms
  displayThread.setInterval(1); // update every 1ms
  wifiThread.setInterval(1); // update every 1ms

  // Startup screen
  matrix.fillScreen(0);
  matrix.drawRGBBitmap(0, 0, logo, 64, 32);
  matrix.show();
}

void loop(void) {
  controller.run();
}

void updateWifi() {
  // TODO implement wifi AP and webserver handling
}

// Update timer timestamps and input handling
void updateTimer() {
  timer.run();
  if (digitalRead(GPIO_START) == LOW) {
    timer.start();
  } else if (digitalRead(GPIO_STOP) == LOW) {
    timer.stop();
  } else if (digitalRead(GPIO_RESET) == LOW) {
    timer.reset();
  }
}

// Update display text based on input from timer
void updateDisplay() {
  sprintf(str, "%s:%s", timer.sec_counter, timer.ms_counter);

  matrix.fillScreen(0);
  matrix.setCursor(textX, textY);
  matrix.print(str);
  matrix.show();
}