#include <Arduino.h>
#include <ESP32Servo.h>
#include <SSD1306Wire.h>

SSD1306Wire lcd(0x3c, SDA, SCL);

// ldr def
#define LDR_BL A17
#define LDR_BR A14
#define LDR_TL A16
#define LDR_TR A15

// #define SERVO_HORIZ 23
// #define SERVO_VERT 18

// Servo Pins: correct orientation
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
    Serial.begin(9600);

    horizontal.attach(SERVO_HORIZ);
    vertical.attach(SERVO_VERT);
    horizontal.write(servoH);
    vertical.write(servoV);
    lcd.init();
    lcd.setFont(ArialMT_Plain_10); // Optional: pick a clear font
    lcd.flipScreenVertically();
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
    // Read and smooth each LDR
    // possible todo: can improve smoothing algorithm, using a low-pass filter
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
    delay(100);
}
