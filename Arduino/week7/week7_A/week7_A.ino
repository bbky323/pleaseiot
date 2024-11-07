#define SWAP 0 // sw access point 
#include <WiFi.h>
#include "time.h"

// Replace with your network credentials
#if SWAP
const char* ssid = "ESP32-AP-GY";
const char* password = "123456789"; // password should be long!!
#else
const char* ssid = "GIYOUNG";
const char* password = "kk323625";
#endif

// NTP settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 9; // GMT +9 (한국 표준시)
const int daylightOffset_sec = 0;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// GPIO variables and state
String output16State = "off";
String output17State = "off";
const int output16 = 16;
const int output17 = 17;

// Function to get local time and return it as a string
String getLocalTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time";
  }
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}

// This function generates the HTML content for real-time clock display
void printLocalTime(WiFiClient client)
{
    client.println("<script>const interval = setInterval(() => {");
    client.println("const now = new Date();");
    client.println("document.getElementById('date').innerHTML = now.toDateString();");
    client.println("document.getElementById('timer').innerHTML = now.toLocaleTimeString();}, 1000);</script>");
    client.println("<div class='bg-slate-600 text-center text-gray-300 p-5 m-10'>");       
    client.println("<span id='date'></span><br/>");
    client.println("<span id='timer' class='text-center countdown font-mono text-5xl text-white'></span></div>");
}

void setup() {
  Serial.begin(115200);
  pinMode(output16, OUTPUT);
  pinMode(output17, OUTPUT);
  digitalWrite(output16, LOW);
  digitalWrite(output17, LOW);

  // Wi-Fi connection
  #if SWAP
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  #else
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  #endif

  // NTP time synchronization
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Start the web server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";  
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Print the received character
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Check if it's a time request (AJAX call)
            if (header.indexOf("GET /time") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println(getLocalTimeString());  // Send the current time
              break;
            }

            // Check for GPIO control (AJAX call)
            if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("GPIO 16 on");
              output16State = "on";
              digitalWrite(output16, HIGH);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("GPIO 16 off");
              output16State = "off";
              digitalWrite(output16, LOW);
            } else if (header.indexOf("GET /17/on") >= 0) {
              Serial.println("GPIO 17 on");
              output17State = "on";
              digitalWrite(output17, HIGH);
            } else if (header.indexOf("GET /17/off") >= 0) {
              Serial.println("GPIO 17 off");
              output17State = "off";
              digitalWrite(output17, LOW);
            }

            // Send HTML page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // HTML page with AJAX and JavaScript
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 { background-color: #555555; }</style></head>");
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Add real-time clock display
            printLocalTime(client);

            // Display GPIO 16 controls
            client.println("<p>GPIO 16 - State " + output16State + "</p>");
            client.println("<button class=\"button\" onclick=\"controlGPIO(16, 'on')\">ON</button>");
            client.println("<button class=\"button button2\" onclick=\"controlGPIO(16, 'off')\">OFF</button>");

            // Display GPIO 17 controls
            client.println("<p>GPIO 17 - State " + output17State + "</p>");
            client.println("<button class=\"button\" onclick=\"controlGPIO(17, 'on')\">ON</button>");
            client.println("<button class=\"button button2\" onclick=\"controlGPIO(17, 'off')\">OFF</button>");

            // Add JavaScript for AJAX
            client.println("<script>");
            client.println("function controlGPIO(pin, state) {");
            client.println("  var xhr = new XMLHttpRequest();");
            client.println("  xhr.open('GET', '/' + pin + '/' + state, true);");
            client.println("  xhr.onload = function() {");
            client.println("    if (xhr.status == 200) {");
            client.println("      console.log('GPIO control success');");
            client.println("    }");
            client.println("  };");
            client.println("  xhr.send();");
            client.println("}");
            client.println("setInterval(function() {");
            client.println("  var xhr = new XMLHttpRequest();");
            client.println("  xhr.open('GET', '/time', true);");
            client.println("  xhr.onload = function() {");
            client.println("    if (xhr.status == 200) {");
            client.println("      document.getElementById('time').innerHTML = xhr.responseText;");
            client.println("    }");
            client.println("  };");
            client.println("  xhr.send();");
            client.println("}, 1000);");
            client.println("</script>");
            
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
