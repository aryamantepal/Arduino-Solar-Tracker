// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <Wire.h>
// #include <SSD1306Wire.h>

// SSD1306Wire lcd(0x3c, SDA, SCL);
// #define SERVO1 23
// #define LIGHTPIN A10

// Servo myservo;

// const int HOME_ANGLE = 0;
// const int ACTIVE_ANGLE = 150;
// const int LIGHT_THRESHOLD = 400;
// int currentAngle = HOME_ANGLE;
// bool isLightDetected = false;

// void setup()
// {
//   Serial.begin(9600);
//   myservo.attach(SERVO1);
//   myservo.write(HOME_ANGLE);

//   lcd.init();
//   lcd.flipScreenVertically();
//   lcd.setFont(ArialMT_Plain_16);

//   Serial.println("Light-sensing servo initialized");
// }

// void loop()
// {
//   // Read light level
//   int lightValue = analogRead(LIGHTPIN);

//   // Determine if light is detected
//   bool lightDetected = (lightValue > LIGHT_THRESHOLD);

//   // Set servo position based on light detection
//   if (lightDetected && !isLightDetected)
//   {
//     // Light just detected - move to active position
//     myservo.write(ACTIVE_ANGLE);
//     currentAngle = ACTIVE_ANGLE;
//     isLightDetected = true;
//     Serial.println("Light ON - Moving to " + String(ACTIVE_ANGLE) + " degrees");
//   }
//   else if (!lightDetected && isLightDetected)
//   {
//     // Light just removed - return to home position
//     myservo.write(HOME_ANGLE);
//     currentAngle = HOME_ANGLE;
//     isLightDetected = false;
//     Serial.println("Light OFF - Returning to " + String(HOME_ANGLE) + " degrees");
//   }

//   // Update display
//   lcd.clear();
//   lcd.drawString(0, 0, "Light: " + String(lightValue));
//   lcd.drawString(0, 20, "Angle: " + String(currentAngle));
//   lcd.drawString(0, 40, lightDetected ? "LIGHT ON" : "LIGHT OFF");
//   lcd.display();

//   delay(50); // Short delay between readings
// }
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <SSD1306Wire.h>

SSD1306Wire lcd(0x3c, SDA, SCL);
#define SERV01 23
#define SERV02 18
#define ldrlt A17
#define ldrrt A14
#define ldrld A16
#define ldrrd A15

Servo horizontal; // Horizontal Servo Motor
int servohori = 180;
int servohoriLimitHigh = 175;
int servohoriLimitLow = 5;

Servo vertical; // Vertical Servo Motor
int servovert = 45;
int servovertLimitHigh = 100;
int servovertLimitLow = 1;

void setup()
{
  horizontal.attach(SERV01);
  vertical.attach(SERV02);
  // horizontal.write(180);
  // vertical.write(45);
  delay(2500);
}

void loop()
{
  int lt = analogRead(ldrlt); // Top left
  int rt = analogRead(ldrrt); // Top right
  int ld = analogRead(ldrld); // Bottom left
  int rd = analogRead(ldrrd); // Bottom right

  int dtime = 10;
  int tol = 500; // Tolerance value for adjustment

  int avt = (lt + rt) / 2; // Average value of top sensors
  int avd = (ld + rd) / 2; // Average value of bottom sensors
  int avl = (lt + ld) / 2; // Average value of left sensors
  int avr = (rt + rd) / 2; // Average value of right sensors

  int dvert = avt - avd;  // Difference between top and bottom
  int dhoriz = avl - avr; // Difference between left and right

  if (abs(dvert) > tol)
  {
    if (avt > avd)
    {
      servovert = ++servovert;
      if (servovert > servovertLimitHigh)
        servovert = servovertLimitHigh;
    }
    else
    {
      servovert = --servovert;
      if (servovert < servovertLimitLow)
        servovert = servovertLimitLow;
    }
    vertical.write(servovert);
  }

  if (abs(dhoriz) > tol)
  {
    if (avl > avr)
    {
      servohori = --servohori;
      if (servohori < servohoriLimitLow)
        servohori = servohoriLimitLow;
    }
    else
    {
      servohori = ++servohori;
      if (servohori > servohoriLimitHigh)
        servohori = servohoriLimitHigh;
    }
    horizontal.write(servohori);
  }

  delay(dtime);
}