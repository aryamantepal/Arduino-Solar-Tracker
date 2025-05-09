// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <Wire.h>
// #include <SSD1306Wire.h>

// SSD1306Wire lcd(0x3c, SDA, SCL);
// #define SERVO1 23
// #define SERVO2 18
// #define LIGHTPIN A10

// Servo myservo;
// Servo myservo2;

// const int HOME_ANGLE = 0;
// const int ACTIVE_ANGLE = 150;
// const int LIGHT_THRESHOLD = 400;
// int currentAngle = HOME_ANGLE;
// bool isLightDetected = false;

// void setup()
// {
//     Serial.begin(9600);
//     myservo.attach(SERVO1);
//     myservo2.attach(SERVO2);
//     myservo.write(HOME_ANGLE);
//     myservo2.write(HOME_ANGLE);

//     lcd.init();
//     lcd.flipScreenVertically();
//     lcd.setFont(ArialMT_Plain_16);

//     Serial.println("Light-sensing servo initialized");
// }

// void loop()
// {
//     // Read light level
//     int lightValue = analogRead(LIGHTPIN);

//     // Determine if light is detected
//     bool lightDetected = (lightValue > LIGHT_THRESHOLD);

//     // Set servo position based on light detection
//     if (lightDetected && !isLightDetected)
//     {
//         // Light just detected - move to active position
//         myservo.write(ACTIVE_ANGLE);
//         myservo2.write(ACTIVE_ANGLE);
//         currentAngle = ACTIVE_ANGLE;
//         isLightDetected = true;
//         Serial.println("Light ON - Moving to " + String(ACTIVE_ANGLE) + " degrees");
//     }
//     else if (!lightDetected && isLightDetected)
//     {
//         // Light just removed - return to home position
//         myservo.write(HOME_ANGLE);
//         myservo2.write(HOME_ANGLE);
//         currentAngle = HOME_ANGLE;
//         isLightDetected = false;
//         Serial.println("Light OFF - Returning to " + String(HOME_ANGLE) + " degrees");
//     }

//     // Update display
//     lcd.clear();
//     lcd.drawString(0, 0, "Light: " + String(lightValue));
//     lcd.drawString(0, 20, "Angle: " + String(currentAngle));
//     lcd.drawString(0, 40, lightDetected ? "LIGHT ON" : "LIGHT OFF");
//     lcd.display();

//     delay(50); // Short delay between readings
// }

// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <Wire.h>
// #include <SSD1306Wire.h>

// SSD1306Wire lcd(0x3c, SDA, SCL);
// #define SERV01 23
// #define SERV02 18
// #define ldrlt A17
// #define ldrrt A14
// #define ldrld A16
// #define ldrrd A15

// Servo horizontal; // Horizontal Servo Motor
// int servohori = 180;
// int servohoriLimitHigh = 175;
// int servohoriLimitLow = 5;

// Servo vertical; // Vertical Servo Motor
// int servovert = 45;
// int servovertLimitHigh = 100;
// int servovertLimitLow = 1;

// void setup()
// {
//     horizontal.attach(SERV01);
//     vertical.attach(SERV02);
//     // horizontal.write(180);
//     // vertical.write(45);
//     delay(2500);
// }

// void loop()
// {
//     int lt = analogRead(ldrlt); // Top left
//     int rt = analogRead(ldrrt); // Top right
//     int ld = analogRead(ldrld); // Bottom left
//     int rd = analogRead(ldrrd); // Bottom right

//     int dtime = 10;
//     int tol = 500; // Tolerance value for adjustment

//     int avt = (lt + rt) / 2; // Average value of top sensors
//     int avd = (ld + rd) / 2; // Average value of bottom sensors
//     int avl = (lt + ld) / 2; // Average value of left sensors
//     int avr = (rt + rd) / 2; // Average value of right sensors

//     int dvert = avt - avd;  // Difference between top and bottom
//     int dhoriz = avl - avr; // Difference between left and right

//     if (abs(dvert) > tol)
//     {
//         if (avt > avd)
//         {
//             servovert = ++servovert;
//             if (servovert > servovertLimitHigh)
//                 servovert = servovertLimitHigh;
//         }
//         else
//         {
//             servovert = --servovert;
//             if (servovert < servovertLimitLow)
//                 servovert = servovertLimitLow;
//         }
//         vertical.write(servovert);
//     }

//     if (abs(dhoriz) > tol)
//     {
//         if (avl > avr)
//         {
//             servohori = --servohori;
//             if (servohori < servohoriLimitLow)
//                 servohori = servohoriLimitLow;
//         }
//         else
//         {
//             servohori = ++servohori;
//             if (servohori > servohoriLimitHigh)
//                 servohori = servohoriLimitHigh;
//         }
//         horizontal.write(servohori);
//     }

//     delay(dtime);
// }

#include <WiFi.h> // header we need to include for WiFi functions
#include <WebServer.h>

const char* ssid = "Totoro_WiFi"; // Change this to sth. unique to you!
const char* pass = "cics290m";
WebServer server(80);  // define server object, at port 80

void on_home() {
  server.send(200, "text/html", "<h1>Hello World!</h1>");
}






#include <Arduino.h>
#include <ESP32Servo.h>
#include <SSD1306Wire.h>

SSD1306Wire lcd(0x3c, SDA, SCL);

// LDR Pins
#define LDR_BL A17
#define LDR_BR A14
#define LDR_TL A16
#define LDR_TR A15

// Servo Pins
// #define SERVO_HORIZ 23
// #define SERVO_VERT 18

#define SERVO_HORIZ 18
#define SERVO_VERT 23
// Servo limits
int servoH = 90;
int servoV = 45;
const int HORIZ_MIN = 5;
const int HORIZ_MAX = 175;
const int VERT_MIN = 1;
const int VERT_MAX = 100;

// Movement config
const int TOLERANCE = 50;        // Light imbalance required to move
const int LIGHT_THRESHOLD = 400; // Ignore below this average light
const int MOVE_STEP = 1;         // Degrees to move per loop

// Smoothing config
const int SMOOTH_WINDOW = 5;
int tlBuffer[SMOOTH_WINDOW], trBuffer[SMOOTH_WINDOW], blBuffer[SMOOTH_WINDOW], brBuffer[SMOOTH_WINDOW];
int bufferIndex = 0;

// Servos
Servo horizontal;
Servo vertical;

void setup()
{
  WiFi.mode(WIFI_AP); // start ESP in AP mode
  WiFi.softAP(ssid, pass); // configure ssid and (optionally) password 
  server.on("/", on_home);  // home callback function
  

  server.begin();  // starts server

    Serial.begin(9600);

    horizontal.attach(SERVO_HORIZ);
    vertical.attach(SERVO_VERT);
    horizontal.write(servoH);
    vertical.write(servoV);
    lcd.init();
    lcd.setFont(ArialMT_Plain_10); // Optional: pick a clear font
    lcd.flipScreenVertically();
    // Init buffers
    for (int i = 0; i < SMOOTH_WINDOW; i++)
    {
        tlBuffer[i] = trBuffer[i] = blBuffer[i] = brBuffer[i] = 0;
    }
}

int smooth(int *buffer, int newValue)
{
    buffer[bufferIndex] = newValue;
    int sum = 0;
    for (int i = 0; i < SMOOTH_WINDOW; i++)
    {
        sum += buffer[i];
    }
    return sum / SMOOTH_WINDOW;
}

void loop()
{ 
  server.handleClient();  // handle client requests, must call frequently
    // Read and smooth each LDR
    int tlRaw = analogRead(LDR_TL);
    int trRaw = analogRead(LDR_TR);
    int blRaw = analogRead(LDR_BL);
    int brRaw = analogRead(LDR_BR);

    int tl = smooth(tlBuffer, tlRaw);
    int tr = smooth(trBuffer, trRaw);
    int bl = smooth(blBuffer, blRaw);
    int br = smooth(brBuffer, brRaw);

    bufferIndex = (bufferIndex + 1) % SMOOTH_WINDOW;

    int avgTop = (tl + tr) / 2;
    int avgBottom = (bl + br) / 2;
    int avgLeft = (tl + bl) / 2;
    int avgRight = (tr + br) / 2;
    int avgAll = (tl + tr + bl + br) / 4;

    Serial.printf("TL: %d TR: %d BL: %d BR: %d\n", tl, tr, bl, br);
    Serial.printf("Top: %d | Bottom: %d | Left: %d | Right: %d | Avg: %d\n", avgTop, avgBottom, avgLeft, avgRight, avgAll);

    // Only move if there's enough light
    if (avgAll > LIGHT_THRESHOLD)
    {
        int diffVert = avgTop - avgBottom;
        int diffHoriz = avgLeft - avgRight;

        if (abs(diffVert) > TOLERANCE)
        {
            if (diffVert > 0 && servoV > VERT_MIN)
            {
                servoV -= MOVE_STEP;
            }
            else if (diffVert < 0 && servoV < VERT_MAX)
            {
                servoV += MOVE_STEP;
            }
            vertical.write(servoV);
        }

        if (abs(diffHoriz) > TOLERANCE)
        {
            if (diffHoriz > 0 && servoH > HORIZ_MIN)
            {
                servoH -= MOVE_STEP;
            }
            else if (diffHoriz < 0 && servoH < HORIZ_MAX)
            {
                servoH += MOVE_STEP;
            }
            horizontal.write(servoH);
        }
    }
    lcd.clear();

    lcd.drawString(0, 0, "TL: " + String(tl) + " TR: " + String(tr));
    lcd.drawString(0, 15, "BL: " + String(bl) + " BR: " + String(br));
    lcd.drawString(0, 30, "H: " + String(servoH) + " V: " + String(servoV));

    bool lightDetected = avgAll > LIGHT_THRESHOLD;
    lcd.drawString(0, 45, lightDetected ? "LIGHT ON" : "LIGHT OFF");

    lcd.display();

    
    String msg = "TL: " + String(tl) + " TR: " + String(tr);
    // server.println(msg);
    // server.print
    server.on("/inline", [](){
      server.send(200, "text/html", "<p>hello TL <var>tl</var> </p> ");
    }); 
    delay(100);
}
