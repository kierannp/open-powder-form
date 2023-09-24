#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "HX711.h"
#include "pid.h"
// #include <AccelStepper.h>
#include <Stepper.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
const float calibration_factor = -686468 / 50;
#define stepPin 6
#define dirPin 7
#define max_stepper_out 5000
#define min_stepper_out 0 
double control_out;
const int stepsPerRevolution = 200;
const double setpoint = .2;


// AccelStepper stepper(1, 6, 7); // (Type of driver: with 2 pins, STEP, DIR)
Stepper stepper(200, 6, 7);
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
HX711 scale;
PID controller(1, max_stepper_out, min_stepper_out, 100.0, 0.0, 0.0 );



void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // stepper.setMaxSpeed(1000);
  // stepper.setMaxSpeed(1000);
	// stepper.setAcceleration(50);
	// stepper.setSpeed(200);
	// stepper.moveTo(200);
  // stepper.setAcceleration(200);
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
  control_out = controller.calculate(setpoint, scale.get_units(5));
  display_to_led("Control_Out: " + String(control_out) +  "\nCurrent: " + String(scale.get_units(5)) +"\nSetPoint:" + String(setpoint));
  // digitalWrite(dirPin, HIGH);
  stepper.setSpeed(control_out);
  stepper.step(10); 
  // stepper.runSpeed();
	// Spin motor slowly
	// for(int x = 0; x < stepsPerRevolution; x++)
	// {
	// 	digitalWrite(stepPin, HIGH);
	// 	delayMicroseconds(int(control_out));
	// 	digitalWrite(stepPin, LOW);
	// 	delayMicroseconds(int(control_out));
	// }
	// delay(1000); // Wait a second
	
	// // Set motor direction counterclockwise
	// digitalWrite(dirPin, LOW);

	// // Spin motor quickly
	// for(int x = 0; x < stepsPerRevolution; x++)
	// {
	// 	digitalWrite(stepPin, HIGH);
	// 	delayMicroseconds(1000);
	// 	digitalWrite(stepPin, LOW);
	// 	delayMicroseconds(1000);
	// }
	delay(1000); // Wait a second
}
