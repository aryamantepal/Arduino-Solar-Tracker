#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <SSD1306Wire.h>

SSD1306Wire lcd(0x3c, SDA, SCL);
#define SERVO1 23
#define LIGHTPIN A10

Servo myservo;

const int HOME_ANGLE = 0;
const int ACTIVE_ANGLE = 150;
const int LIGHT_THRESHOLD = 400;
int currentAngle = HOME_ANGLE;
bool isLightDetected = false;

void setup()
{
  Serial.begin(9600);
  myservo.attach(SERVO1);
  myservo.write(HOME_ANGLE);

  lcd.init();
  lcd.flipScreenVertically();
  lcd.setFont(ArialMT_Plain_16);

  Serial.println("Light-sensing servo initialized");
}

void loop()
{
  // Read light level
  int lightValue = analogRead(LIGHTPIN);

  // Determine if light is detected
  bool lightDetected = (lightValue > LIGHT_THRESHOLD);

  // Set servo position based on light detection
  if (lightDetected && !isLightDetected)
  {
    // Light just detected - move to active position
    myservo.write(ACTIVE_ANGLE);
    currentAngle = ACTIVE_ANGLE;
    isLightDetected = true;
    Serial.println("Light ON - Moving to " + String(ACTIVE_ANGLE) + " degrees");
  }
  else if (!lightDetected && isLightDetected)
  {
    // Light just removed - return to home position
    myservo.write(HOME_ANGLE);
    currentAngle = HOME_ANGLE;
    isLightDetected = false;
    Serial.println("Light OFF - Returning to " + String(HOME_ANGLE) + " degrees");
  }

  // Update display
  lcd.clear();
  lcd.drawString(0, 0, "Light: " + String(lightValue));
  lcd.drawString(0, 20, "Angle: " + String(currentAngle));
  lcd.drawString(0, 40, lightDetected ? "LIGHT ON" : "LIGHT OFF");
  lcd.display();

  delay(50); // Short delay between readings
}