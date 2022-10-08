#include <Adafruit_Protomatter.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <WiFi.h>
#include "timer.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"  
#include "logo.h"

// Color definitions used for setting text and background color
#define BLACK    0x0000
#define RED      0xF800
#define GREEN    0x07E0
#define WHITE    0xFFFF

const uint16_t logoStartupTime = 5000; // Variable decides how long the logo is shown at startup

// Display controller pins
uint8_t rgbPins[]  = {2, 3, 4, 5, 8, 9};
uint8_t addrPins[] = {10, 16, 18, 20, 22};
uint8_t clockPin   = 11;
uint8_t latchPin   = 12;
uint8_t oePin      = 13;

// Button/input pins
uint8_t startBtnPin = 15;           // Button that triggers a timer.start() call if triggered
uint8_t stopBtnPin = 14;            // Button that triggers a timer.stop() call if triggered
uint8_t resetBtnPin = 17;           // Button that triggers a timer.reset() call if triggered
uint8_t sensPot = 26;               // Potentimeter that controls the sensitivity of the vibration sensor
uint8_t judgeVibSensor = 27;        // Vibration sensor that triggers a timer.stop() call if triggered
uint8_t participantVibSensor = 28;  // Vibration sensor that triggers a timer.start() call if triggered

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
char     str[8];                // Buffer to hold timer text "XXX:YYY\0"

Timer timer;

int port = 80;
char ssid[] = "Bamses Øhlhäfvarstoppur";
char pass[] = "test1234";
int status = WL_IDLE_STATUS;
WiFiServer server(port);

String httpRequest;

Thread timerThread = Thread();
Thread displayThread = Thread();
Thread wifiThread = Thread();
StaticThreadController<3> controller (&timerThread, &displayThread, &wifiThread);

void setup(void) {
  Serial.begin(115200);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true); // Don't continue
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  status = WiFi.beginAP(ssid, pass);

  if (status != WL_CONNECTED) {
    Serial.println("Creating access point failed");
    
    while (true); // Don't continue
  }

  delay(10000);

  server.begin();

  printWiFiStatus();

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

  pinMode(startBtnPin, INPUT_PULLUP);
  pinMode(stopBtnPin, INPUT_PULLUP);
  pinMode(resetBtnPin, INPUT_PULLUP);

  timerThread.onRun(updateTimer);
  displayThread.onRun(updateDisplay);
  wifiThread.onRun(updateWifi);
  timerThread.setInterval(1); // update every 1ms
  displayThread.setInterval(1); // update every 1ms
  wifiThread.setInterval(100); // update every 100ms

  // Startup screen
  matrix.fillScreen(BLACK);
  matrix.drawRGBBitmap(0, 0, logo, 64, 32);
  matrix.show();
  sleep_ms(logoStartupTime);
}

void printWiFiStatus() {

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void loop(void) {
  controller.run();
  //updateWifi();
}

void updateWifi() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {

    Serial.println("new client");
    // An http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        httpRequest += c;

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          Serial.println(httpRequest); //print client http request

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<title>Bamses Øhlhäfvarstoppur Admin panel</title>");
          client.println("<h1 style=\"color: #5e9ca0;\">Tidtagarkontrollpanel</h1>");
          client.println("<a href=/?start>START TIMER</a>");
          client.println("<a href=/?stop>STOP TIMER</a>");
          client.println("<a href=/?reset>RESET TIMER</a>");
          client.println("<br>");
          // TODO add timer value
          client.println("</html>");

          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    // give the web browser time to receive the data
    delay(1);

    // close the connection:

    client.stop();
    Serial.println("client disconnected");

    if (httpRequest.indexOf("?start") > 0 ) {
      timer.start();
    }

    if (httpRequest.indexOf("?stop") > 0 ) {
      timer.stop();
    }

    if (httpRequest.indexOf("?reset") > 0 ) {
      timer.reset();
    }

    httpRequest = "";
  }
}

// Update timer timestamps and input handling
void updateTimer() {
  uint16_t sensPotValue = analogRead(sensPot);
  uint16_t judgeVibSensorValue = analogRead(judgeVibSensor);
  uint16_t participantVibSensorValue = analogRead(participantVibSensor);

  Serial.print("sensor: ");
  Serial.println(judgeVibSensorValue);
  timer.run();
   
  if (digitalRead(startBtnPin) == LOW || (judgeVibSensorValue >= sensPotValue)) { // Timer can be started from button and judge's vibration sensor
    timer.start();
  } else if ((digitalRead(stopBtnPin) == LOW)  || (participantVibSensorValue >= sensPotValue)) { // Timer can be stopped from button and participants's vibration sensor
    timer.stop();
  } else if (digitalRead(resetBtnPin) == LOW) {
    timer.reset();
  }
}

// Update display text based on input from timer
void updateDisplay() {
  timer.run();
  sprintf(str, "%s:%s", timer.sec_counter, timer.ms_counter);

  matrix.fillScreen(0);
  matrix.setCursor(textX, textY);

  // Timer text default white (init and reset).
  uint16_t textColor = WHITE;

  switch (timer.state) {
    case Run:
      textColor = GREEN;
      break;

    case Pause:
      textColor = RED;
      break;
  }

  matrix.setTextColor(textColor);
  matrix.print(str);
  matrix.show();
}
