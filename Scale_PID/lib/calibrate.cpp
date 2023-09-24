#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "HX711.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

const float calibration_factor = -686468 / 50;

HX711 scale;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void loop() {
  auto display_to_led = [](String message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(message);
    display.display();
  };
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 10);
    // display.println("dicks");

  if (scale.is_ready()) {
    scale.set_scale();  
    display_to_led("Tare... remove any weights from the scale.");  
    delay(5000);
    scale.tare();
    display_to_led("Tare done...");
    delay(2000);
    display_to_led("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    display_to_led("Result: " + String(reading));
    delay(100000);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}