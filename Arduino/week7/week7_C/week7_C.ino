#include <EEPROM.h>
#include <WiFi.h>
#include "time.h"

// Wi-Fi settings
const char* ssid = "GIYOUNG";
const char* password = "kk323625";
WiFiServer server(80);

// NTP settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 9; // KST
const int daylightOffset_sec = 0;

// GPIO and PWM settings for buzzer
const int buzPin = 23;
const int ledChannel = 0;
const int resolution = 8;
const int duty = 128;

// EEPROM signature for melody storage
const int signature1 = 0xAA;
const int signature2 = 0x55;
const int startEEPROM = 3;

// Alarm time variables
int alarmHour = -1;
int alarmMinute = -1;
bool alarmSet = false;

// Note and duration arrays
int nFrq[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
int nDur[] = {2000, 1500, 1000, 750, 500, 375, 250};

// Web request variables
String header;

// Functions for melody playback
void playNote(int note, int dur) {
  if (note == -1) {
    ledcWrite(ledChannel, 0);  // Stop note
  } else {
    ledcSetup(ledChannel, nFrq[note], resolution);
    ledcWrite(ledChannel, duty);
  }
  delay(nDur[dur]);
}

void saveMusicToEEPROM(String music) {
  int index = startEEPROM;
  int count = 0;

  for (int i = 0; i < music.length(); i += 2) {
    int note = music[i] - '0';
    int dur = music[i + 1] - '0';
    EEPROM.write(index++, note);
    EEPROM.write(index++, dur);
    count++;
  }

  EEPROM.write(0, signature1);
  EEPROM.write(1, signature2);
  EEPROM.write(2, count);
  EEPROM.commit();
  Serial.println("Music has been saved successfully.");
}

void playMusicFromEEPROM() {
  if (EEPROM.read(0) == signature1 && EEPROM.read(1) == signature2) {
    int count = EEPROM.read(2);
    int index = startEEPROM;

    for (int i = 0; i < count; i++) {
      int note = EEPROM.read(index++);
      int dur = EEPROM.read(index++);
      playNote(note, dur);
    }
  }
}

// Time and alarm functions
void printLocalTime(WiFiClient client) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    client.println("Failed to obtain time");
    return;
  }
  String currentTimeStr = String(timeinfo.tm_year + 1900) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_mday) + " " +
                          String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
  client.println("<p>Current Time: " + currentTimeStr + "</p>");
}

void checkAlarm() {
  if (!alarmSet) return;

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;

    if (currentHour == alarmHour && currentMinute == alarmMinute) {
      Serial.println("Alarm triggered! Playing melody.");
      playMusicFromEEPROM();
      alarmSet = false;  // Disable the alarm after it triggers
    }
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  pinMode(buzPin, OUTPUT);
  ledcAttachPin(buzPin, ledChannel);

  // Wi-Fi setup
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // Time setup
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Save the input melody to EEPROM on startup
  saveMusicToEEPROM("04060604044476764404767644767644040404,474744404747474,474744404747474,474744404,0c474c474442404,4");
}

void loop() {
  checkAlarm();

  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} ");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; ");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;} ");
            client.println(".button2 { background-color: #555555; }</style></head>");
            client.println("<body><h1>ESP32 Alarm Clock</h1>");

            // Alarm setting form
            client.println("<h2>Set Alarm</h2>");
            client.println("<form action=\"/setAlarm\" method=\"GET\">");
            client.println("Hour: <input type=\"number\" name=\"hour\" min=\"0\" max=\"23\"><br>");
            client.println("Minute: <input type=\"number\" name=\"minute\" min=\"0\" max=\"59\"><br>");
            client.println("<input type=\"submit\" value=\"Set Alarm\" class=\"button\"></form>");

            printLocalTime(client);

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

    // Parse the alarm setting
    if (header.indexOf("GET /setAlarm") >= 0) {
      int hourIndex = header.indexOf("hour=") + 5;
      int minuteIndex = header.indexOf("minute=") + 7;
      alarmHour = header.substring(hourIndex, header.indexOf("&", hourIndex)).toInt();
      alarmMinute = header.substring(minuteIndex, header.indexOf(" ", minuteIndex)).toInt();
      alarmSet = true;
      Serial.printf("Alarm set for %02d:%02d\n", alarmHour, alarmMinute);
    }

    // Clear the header
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}
