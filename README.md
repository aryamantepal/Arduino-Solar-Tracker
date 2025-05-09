🌞 Arduino Solar Tracker (ESP32)
This project is a dual-axis solar tracking system powered by an ESP32, which uses LDRs, servo motors, and an OLED display to automatically align a solar panel with the sun for optimal light exposure.

🔧 Features
Dual-axis tracking using 4 LDR sensors

Smooth servo motion with light threshold and tolerance settings

OLED display showing live light sensor readings and servo positions

Buffered sensor reading for noise reduction

🛠 Hardware Required
ESP32 development board

4 × LDRs (Light Dependent Resistors)

4 × 10kΩ resistors (for voltage divider with LDRs)

2 × Servo motors (SG90 or similar)

0.96” I2C OLED display (SSD1306 128x64)

Breadboard + jumper wires

Solar panel or mock mount for testing

⚙️ Pin Configuration
Component	Pin
LDR Top-Left	A16
LDR Top-Right	A15
LDR Bottom-Left	A17
LDR Bottom-Right	A14
Servo Horizontal	GPIO 18
Servo Vertical	GPIO 23
OLED Display	SDA, SCL (default ESP32 I2C pins)

🧠 Code Logic Summary
Reads light levels from 4 LDRs placed in quadrants.

Averages top vs bottom and left vs right values.

Moves vertical/horizontal servos to minimize the light imbalance.

Updates a small OLED with sensor values and tracking status.

Ignores movement if overall brightness is too low.

📦 Libraries Used
ESP32Servo

SSD1306Wire

Install these libraries via PlatformIO or the Arduino Library Manager.

🖼️ OLED Display Output
vbnet
Copy
Edit
TL: 789  TR: 801  
BL: 670  BR: 690  
H: 90    V: 45  
LIGHT ON
🔄 Tuning Parameters (in code)
TOLERANCE: Minimum light imbalance to move servos (default: 50)

LIGHT_THRESHOLD: Minimum light level required to activate tracking (default: 400)

MOVE_STEP: Degrees of servo movement per update (default: 1)

SMOOTH_WINDOW: Number of samples for light smoothing (default: 5)

📸 Demo
Add images or videos of your setup here.

📂 File Structure
css
Copy
Edit
/src
 └── main.cpp       ← Arduino logic for tracking
✅ Future Improvements
Add low-pass filter instead of fixed-size buffer

Auto-calibrate servo limits

Add real-time clock (RTC) fallback for nighttime reset

Solar output monitoring & logging

