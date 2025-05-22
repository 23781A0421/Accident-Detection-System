Accident Detection System using ESP8266 & MPU6050

A real-time accident detection system built using ESP8266 (NodeMCU), MPU6050 (accelerometer + gyroscope), Discord Webhook for alerts, and ThingSpeak for live data logging and monitoring. This project simulates the system on Wokwi using ESP32 but is designed for real-world use with ESP8266.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
Features
- Real-time accident detection using motion sensors.
- Sends alerts via **Discord Webhook** within 30 seconds.
- Logs and visualizes crash data using **ThingSpeak** dashboard.
- Includes manual cancel button to stop false alerts.
- Simulates easily in **Wokwi** and works with real **ESP8266** hardware.
----------------------------------------------------------------------------
Components Used:
### Hardware
- ESP8266 NodeMCU (real implementation)
- ESP32 DevKit (Wokwi simulation)
- MPU6050 (Accelerometer + Gyroscope)
- Buzzer
- LED
- Push Button
- 220Ω Resistor (for LED)
- Jumper Wires & Breadboard

### Software/Platforms
- Arduino IDE
- Wokwi (Simulation)
- Discord (Webhook for alerts)
- ThingSpeak (Data visualization)
- Fritzing (optional, for PCB design)
--------------------------------------------------
Setup Instructions:

### 1. Discord Webhook Setup
- Create a server → Channel → Integrations → New Webhook
- Copy Webhook URL and paste it in `crash_detection_esp8266_code.ino` or simulation code.

### 2. ThingSpeak Setup
- Create an account → New Channel
- Note the **Write API Key** and paste it into the code.

### 3. Wokwi Simulation (ESP32)
- Go to [Wokwi](https://wokwi.com/projects/431580179335709697)
- Use `diagram.json` and `crash_detection_esp32_code.ino`
- Simulate crash by adjusting acceleration
- Test Discord and ThingSpeak output

### 4. 🔌 Real ESP8266 Hardware Connections
| Component   | Pin (ESP8266) |
|-------------|---------------|
| MPU6050 SDA | D8            |
| MPU6050 SCL | D7            |
| Buzzer +    | D5            |
| Button      | D6            |
| LED         | D4 (with 220Ω)|
--------------------------------------------------------
How It Works:
1. Reads acceleration from MPU6050.
2. If G-force exceeds **1.5g**, system assumes a crash.
3. Countdown starts for 30 seconds (LED ON, buzzer ON).
4. If not canceled via button → sends **final alert** to Discord.
5. Periodically sends data (magnitude, status, emergency flag) to ThingSpeak.
--------------------------------------------------------------------------------------
Key Thresholds:
const float crashThreshold = 1.5;     // >1.5g = Crash
const float loggingThreshold = 0.3;  // >0.3g = Log to ThingSpeak


Project for L&T EduTech Course: Applied Industrial IoT
Project Members:
23781A0421 – B.Revanth
23781A0422 – B.Sumanth
23781A0423 – B. Venkataramana Reddy
23781A0424 – B.Karthik Reddy
23781A0425 – B.Kishore