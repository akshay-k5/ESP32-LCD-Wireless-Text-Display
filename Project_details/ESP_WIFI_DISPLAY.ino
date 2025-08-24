//This is a Sample Program to print text on LCD Screen Wirelessly using ESP32 wroom 
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, change to 0x3F if needed

// WiFi Access Point credentials
const char *ap_ssid = "ESP32_Hotspot"; //Random SSID
const char *ap_password = "12345678";  // Min 8 characters

// Web server on port 80
WebServer server(80);

// Message handling
String currentMsg = "";
unsigned long lastScroll = 0;
int scrollPos = 0;
int scrollDelay = 300;   // ms per shift

// Wi-Fi details scrolling
String wifiDetails = "";
int wifiScrollPos = 0;

// HTML Page
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LCD Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; background: #f4f4f9; margin: 0; padding: 20px; }
    h2 { color: #333; }
    input[type="text"] { padding: 10px; width: 60%; font-size: 16px; border-radius: 8px; border: 1px solid #ccc; }
    input[type="submit"] { padding: 10px 20px; font-size: 16px; border: none; border-radius: 8px; background: #007BFF; color: white; cursor: pointer; }
    input[type="submit"]:hover { background: #0056b3; }
    #status { margin-top: 20px; font-weight: bold; color: green; }
    footer { margin-top: 40px; font-size: 12px; color: #777; }
  </style>
</head>
<body>
  <h2>ESP32 LCD Wireless Control</h2>
  <form id="msgForm">
    <input type="text" id="msg" name="msg" placeholder="Enter message">
    <input type="submit" value="Send">
  </form>
  <div id="status"></div>
  <footer>&copy; akshay-k5</footer>
  
  <script>
    const form = document.getElementById('msgForm');
    form.addEventListener('submit', function(e) {
      e.preventDefault();
      const msg = document.getElementById('msg').value;
      fetch('/send?msg=' + encodeURIComponent(msg))
        .then(response => response.text())
        .then(data => {
          document.getElementById('status').innerHTML = "Sent to LCD: " + msg;
          document.getElementById('msg').value = "";
        });
    });
  </script>
</body>
</html>
)rawliteral";

// Handle root page
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handle message input
void handleSend() {
  if (server.hasArg("msg")) {
    currentMsg = server.arg("msg") + "   ";  // add gap for smooth loop
    scrollPos = 0;  // restart scrolling
    Serial.print("[WEB] Message received: ");
    Serial.println(currentMsg);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(200, "text/plain", "No message");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize LCD
  Wire.begin(21, 22);  
  lcd.init();
  lcd.backlight();

  // Setup Wi-Fi as AP
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress IP = WiFi.softAPIP();

  Serial.print("[WIFI] AP started: "); Serial.println(ap_ssid);
  Serial.print("[WIFI] Password: "); Serial.println(ap_password);
  Serial.print("[WIFI] AP IP address: "); Serial.println(IP);

  // Prepare Wi-Fi details scrolling string
  wifiDetails = "SSID: " + String(ap_ssid) + "   " +
                "PASS: " + String(ap_password) + "   " +
                "IP: " + IP.toString() + "   ";

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
  Serial.println("[SERVER] Web server started");
}

void loop() {
  server.handleClient();

  if (currentMsg.length() > 0) {
    // Scroll user message
    if (millis() - lastScroll > scrollDelay) {
      lastScroll = millis();
      lcd.clear();

      if (currentMsg.length() <= 16) {
        lcd.setCursor(0, 0);
        lcd.print(currentMsg);
      } else {
        if (scrollPos > currentMsg.length() - 16) scrollPos = 0;
        String window = currentMsg.substring(scrollPos, scrollPos + 16);
        lcd.setCursor(0, 0);
        lcd.print(window);
        scrollPos++;
      }
    }
  } else {
    // Scroll Wi-Fi details endlessly until user sends text
    if (millis() - lastScroll > scrollDelay) {
      lastScroll = millis();
      lcd.clear();

      if (wifiScrollPos > wifiDetails.length() - 16) wifiScrollPos = 0;
      String window = wifiDetails.substring(wifiScrollPos, wifiScrollPos + 16);
      lcd.setCursor(0, 0);
      lcd.print(window);
      wifiScrollPos++;
    }
  }
}
