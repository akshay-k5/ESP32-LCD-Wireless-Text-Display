# ESP32 LCD Wireless Text Display  

This project demonstrates how to **control a 16x2 I²C LCD wirelessly using an ESP32**.  
The ESP32 hosts a Wi-Fi access point and a web server, allowing you to send custom messages from any device (mobile/PC) via a browser.  

---

## ✨ Features
- Runs ESP32 in **Access Point (AP) mode** (no external Wi-Fi required).  
- Built-in **webpage UI** to send text to the LCD.  
- Text longer than **16 characters scrolls smoothly** across the LCD.  
- **Wi-Fi details (SSID, Password, IP)** scroll on LCD at boot until a new message is sent.  
- **Interrupts scrolling immediately** when new message is received.  
- Endless scrolling of both Wi-Fi details and user messages.  

---

## 🔧 Hardware Requirements
- ESP32-WROOM board  
- 16x2 LCD with I²C backpack (default address: `0x27`, may vary)  
- Jumper wires  

---

## 🔌 Connections (ESP32 → LCD via I²C)

| ESP32 Pin | LCD Pin |
|-----------|---------|
| 3.3V / 5V | VCC     |
| GND       | GND     |
| GPIO 21   | SDA     |
| GPIO 22   | SCL     |

---

## 🌐 Web Interface
- ESP32 creates a Wi-Fi hotspot:  
  - **SSID:** `ESP32_Hotspot`  
  - **Password:** `12345678`  
- Connect your device to this Wi-Fi.  
- Open browser → go to the IP Displayed in the Screen  
- Enter your message and hit **Send**.  
- The LCD will instantly update and start scrolling your message.  

