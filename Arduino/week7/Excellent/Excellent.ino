#include <EEPROM.h>
#include <WiFi.h>
#include "time.h"

#define SWAP 0 // sw access point 

// 네트워크 자격 증명 설정 (SSID, 비밀번호)
#if SWAP
const char* ssid = "ESP32-AP-GY";
const char* password = "123456789";
#else
const char* ssid = "NEXT-504N_B9B4F0";
const char* password = "kk323625";
#endif

WiFiServer server(12000); // 웹 서버 포트 번호를 12000으로 설정

String header; // HTTP 요청을 저장할 변수
String output16State = "off"; // 현재 출력 상태를 저장할 보조 변수
String output17State = "off"; // 현재 출력 상태를 저장할 보조 변수

const int output16 = 16;  // GPIO 16에 연결된 장치 (예: LED)
const int output17 = 17;  // GPIO 17에 연결된 장치 (예: LED)


unsigned long currentTime = millis(); // 현재 시간
unsigned long previousTime = 0; // 이전 시간
const long timeoutTime = 2000; // 타임아웃 시간을 밀리초로 설정

// NTP 설정
const char* ntpServer = "pool.ntp.org";  // NTP 서버 주소
const long gmtOffset_sec = 3600 * 9;  // GMT 오프셋 (초 단위), 서울의 경우 9시간 오프셋
const int daylightOffset_sec = 0;   // 서머타임 오프셋 (초 단위), 현재 서머타임 사용 안함

// PWM 설정
const int ledChannel = 0;
const int resolution = 10;
const int buzPin = 23;
const int duty = 128;

// EEPROM 관련 변수
const int maxSize = 100;
int noteCount = 0;
const int signature1 = 0xAA;
const int signature2 = 0x55;

// 알람 설정 관련 변수
int alarmHour = -1; // 설정된 알람 시간 (시)
int alarmMinute = -1; // 설정된 알람 시간 (분)
bool alarmSet = false; // 알람 설정 여부

// 음과 지속 시간
int nFrq[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 0};
int nDur[] = { 2000, 1500, 1000, 750, 500, 375, 250 };

// EEPROM에서 노트 및 지속시간 읽기
void playMusicFromEEPROM() {
  int savedNoteCount = EEPROM.read(2);
  for (int i = 0; i < savedNoteCount; i++) {
    int note = EEPROM.read(3 + i * 2);
    int dur = EEPROM.read(3 + i * 2 + 1);
    if (note != 13) {
      ledcSetup(ledChannel, nFrq[note], resolution);
      ledcWrite(ledChannel, duty);
    } else {
      ledcWrite(ledChannel, 0);
    }
    delay(nDur[dur]);
    ledcWrite(ledChannel, 0);
  }
}

// 로컬 시간을 출력하는 함수
String getLocalTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {  // 시간 정보를 가져오지 못한 경우
    return "시간 정보를 얻지 못했습니다";
  }
  // 시간 문자열로 변환
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}

// 알람 시간이 되었는지 확인하는 함수
void checkAlarm() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    if (alarmSet && timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute) {
      Serial.println("알람 시간이 되었습니다! 멜로디 재생 시작.");
      playMusicFromEEPROM();
      alarmSet = false;  // 알람이 울리면 알람 해제
    }
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);  // EEPROM 초기화
  pinMode(output16, OUTPUT);
  pinMode(output17, OUTPUT);
  digitalWrite(output16, LOW);
  digitalWrite(output17, LOW);

  // attach the channel to the GPIOs
  ledcAttachPin(buzPin, ledChannel);
  
  // SSID와 비밀번호로 Wi-Fi 네트워크에 연결
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // 로컬 IP 주소를 출력하고 웹 서버를 시작
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // NTP 서버에서 시간 설정 및 가져오기
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // EEPROM에서 저장된 음악 유효성 확인
  if (EEPROM.read(0) == signature1 && EEPROM.read(1) == signature2) {
    Serial.println("EEPROM에 저장된 유효한 멜로디를 발견했습니다.");
  } else {
    Serial.println("EEPROM에 저장된 유효한 멜로디가 없습니다.");
  }

  server.begin();
}

void loop() {
  checkAlarm();  // 알람 시간 확인

  WiFiClient client = server.available();  // 들어오는 클라이언트를 대기
  if (client) {  // 새로운 클라이언트가 연결되면,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("새 클라이언트 연결됨.");
    String currentLine = "";  // 클라이언트에서 들어오는 데이터를 저장할 문자열 생성
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // 클라이언트가 연결된 동안
      currentTime = millis();
      if (client.available()) {  // 클라이언트에서 읽을 데이터가 있으면,
        char c = client.read();  // 한 바이트를 읽고
        Serial.write(c);  // 시리얼 모니터에 출력
        header += c;
        if (c == '\n') {  // 바이트가 줄 바꿈 문자면
          if (currentLine.length() == 0) {
            // 시간 요청 처리
            if (header.indexOf("GET /time") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/plain");
              client.println("Connection: close");
              client.println();
              client.println(getLocalTimeString());  // 현재 시간 전송
            } else {
              // 알람 설정 처리
              if (header.indexOf("GET /setAlarm") >= 0) {
                String h = header.substring(header.indexOf("hour=") + 5, header.indexOf("&"));
                String m = header.substring(header.indexOf("minute=") + 7, header.indexOf(" HTTP/"));
                alarmHour = h.toInt();
                alarmMinute = m.toInt();
                alarmSet = true;
                Serial.println("알람이 설정되었습니다: " + String(alarmHour) + ":" + String(alarmMinute));
              }

              if (header.indexOf("GET /16/on") >= 0) {
    Serial.println("GPIO 16 켜기");
    output16State = "on";
    digitalWrite(output16, HIGH); // GPIO 16 HIGH -> LED ON
} else if (header.indexOf("GET /16/off") >= 0) {
    Serial.println("GPIO 16 끄기");
    output16State = "off";
    digitalWrite(output16, LOW);  // GPIO 16 LOW -> LED OFF
} else if (header.indexOf("GET /17/on") >= 0) {
    Serial.println("GPIO 17 켜기");
    output17State = "on";
    digitalWrite(output17, HIGH); // GPIO 17 HIGH -> LED ON
} else if (header.indexOf("GET /17/off") >= 0) {
    Serial.println("GPIO 17 끄기");
    output17State = "off";
    digitalWrite(output17, LOW);  // GPIO 17 LOW -> LED OFF
}

              // 전체 페이지 응답 (단일 출력)
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style></head>");
              client.println("<body><h1>ESP32 Alarm Clock</h1>");

              // Display GPIO 16 controls
client.println("<p>GPIO 16 - State " + output16State + "</p>");
client.println("<button class=\"button\" onclick=\"controlGPIO(16, 'on')\">ON</button>");
client.println("<button class=\"button button2\" onclick=\"controlGPIO(16, 'off')\">OFF</button>");

// Display GPIO 17 controls
client.println("<p>GPIO 17 - State " + output17State + "</p>");
client.println("<button class=\"button\" onclick=\"controlGPIO(17, 'on')\">ON</button>");
client.println("<button class=\"button button2\" onclick=\"controlGPIO(17, 'off')\">OFF</button>");

// JavaScript for button control
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
client.println("</script>");

              
              // 시간은 AJAX로만 업데이트
              client.println("<p>현재 시간: <span id=\"time\">로딩 중...</span></p>");
              client.println("<script>");
              client.println("setInterval(function() {");
              client.println("var xhttp = new XMLHttpRequest();");
              client.println("xhttp.onreadystatechange = function() {");
              client.println("if (this.readyState == 4 && this.status == 200) {");
              client.println("document.getElementById(\"time\").innerHTML = this.responseText;");
              client.println("}");
              client.println("};");
              client.println("xhttp.open(\"GET\", \"/time\", true);");
              client.println("xhttp.send();");
              client.println("}, 1000);");  // 1초마다 시간 업데이트
              client.println("</script>");
              
              // 알람 설정 입력 폼
              client.println("<form action=\"/setAlarm\" method=\"GET\">");
              client.println("시간 (24시간 형식): <input type=\"number\" name=\"hour\" min=\"0\" max=\"23\" required><br>");
              client.println("분: <input type=\"number\" name=\"minute\" min=\"0\" max=\"59\" required><br>");
              client.println("<input type=\"submit\" value=\"알람 설정\" class=\"button\"></form>");

              // 설정된 알람 시간 출력
              if (alarmSet) {
                client.println("<p>알람이 설정되었습니다: " + String(alarmHour) + ":" + String(alarmMinute) + "</p>");
              } else {
                client.println("<p>알람이 설정되지 않았습니다.</p>");
              }
              
              client.println("</body></html>");
            }
            // HTTP 응답은 빈 줄로 끝남
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
    header = "";
    client.stop();
    Serial.println("클라이언트 연결 종료.");
  }
}
