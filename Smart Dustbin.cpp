#include <CheapStepper.h>
#include <Servo.h>

// Components
Servo servo1;
CheapStepper stepper(8, 9, 10, 11);

// Pin Definitions
#define ir 5
#define proxi 6
#define RAIN_SENSOR_PIN 2  // Digital pin for the rain sensor

// Variables
bool motorMovedProxi = false;
bool motorMovedIR = false;

void setup() {
  Serial.begin(9600);
  pinMode(proxi, INPUT_PULLUP);
  pinMode(ir, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT_PULLUP);  // Set rain sensor pin as a digital input with pull-up resistor
  servo1.attach(7);

  stepper.setRpm(17); 
  servo1.write(70); // Initialize to closed position
  delay(1000);
}

void loop() {
  // Read sensor states
  int proximityState = digitalRead(proxi);
  int irState = digitalRead(ir);
  int rainState = digitalRead(RAIN_SENSOR_PIN); // Read the rain sensor state

  // *Rain Sensor Action*
  if (rainState == LOW) {  // LOW means rain is detected (assuming active LOW logic for rain detection)
    // Rotate stepper motor 180 degrees
    stepper.moveDegreesCW(180);  
    delay(500);

    // Open servo
    servo1.write(180);
    delay(1000);

    // Close servo
    servo1.write(70);
    delay(500);

    // Rotate stepper motor back 180 degrees
    stepper.moveDegreesCCW(180);  
    return;  // Exit loop after detecting rain
  }

  // *Proximity Sensor Action*
  if (proximityState == LOW && !motorMovedProxi) {
    stepper.moveDegreesCW(240);
    delay(500);

    servo1.write(180); // Open servo
    delay(1000);
    servo1.write(70);  // Close servo
    delay(500);

    stepper.moveDegreesCCW(240);
    motorMovedProxi = true;
  } else if (proximityState == HIGH) {
    motorMovedProxi = false; // Reset state when sensor is idle
  }

  // *IR Sensor Action*
  if (irState == LOW && !motorMovedIR) {
    servo1.write(180); // Open servo
    delay(1000);
    servo1.write(70);  // Close servo
    delay(500);

    motorMovedIR = true;
  } else if (irState == HIGH) {
    motorMovedIR = false; // Reset state when sensor is idle
  }
}