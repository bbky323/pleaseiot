#include <AWS_IOT.h> //AWS_IOT.zip 라이브러리를 include
#include <WiFi.h>

AWS_IOT testButton; // 변수 선언
const char* ssid = "GIYOUNG"; //Wi-Fi 연결
const char* password = "kk323625";
char HOST_ADDRESS[]="a10tzgndlmewra-ats.iot.ap-northeast-2.amazonaws.com"; //AWS에 만들어놓은 thing과 연결하기 위해 주소 입력, a10tzgndlmewra-ats.iot.ap-northeast-2.amazonaws.com
char CLIENT_ID[]= "ESP32_testButton ";
char sTOPIC_NAME[]= "esp32/buttAck"; // subscribe topic name
char pTOPIC_NAME[]= "esp32/button"; // publish topic name

int status = WL_IDLE_STATUS;
int msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
const int buttonPin = 15; // pushbutton pin
unsigned long preMil = 0;
const long intMil = 5000; // 버튼 바운싱 이슈 때문에 5초간은 안 눌리게 설정

//콜백 함수 설정
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad) //이 파라미터들은 interrupt에 들어있는 정보들
{
  strncpy(rcvdPayload,payLoad,payloadLen); // rcvdPayload라는 버퍼에 복사, 다음에 오는 메세지가 overwriting 될 수 있기 떄문에
  rcvdPayload[payloadLen] = 0;
  msgReceived = 1; // flag 설정
}

// Wi-Fi 세팅, 굳이 할 필요는 없지만 wi-fi 연결을 더 원할하게 하기 위해서 하는 거라고 함.
void setup() {
  Serial.begin(115200);
  //++choi This is here to force the ESP32 to reset the WiFi and initialize correctly.
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode()); //++choi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to wifi");

  //AWS와 연결
  if(testButton.connect(HOST_ADDRESS,CLIENT_ID)== 0) {
    Serial.println("Connected to AWS");
    delay(1000);
    if(0==testButton.subscribe(sTOPIC_NAME,mySubCallBackHandler)) { //sTOPIC_NAME이라는 topic이 오면, mySubCallBackHandler라는 함수를 호춣해달라는 registering 부분, subscribe 하는 것.
      Serial.println("Subscribe Successfull");
    }
    else {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while(1);
    }
  }
  else {
    Serial.println("AWS connection failed, Check the HOST Address");
    while(1);
  }
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  delay(2000);
}

//loop문 설정
void loop() {
  if(msgReceived == 1) { //콜백 함수에서 flag를 설정해놓은 상태
    msgReceived = 0; // 다음에 flag를 써야하기 때문에 0으로 초기화
    Serial.print("Received Message:");
    Serial.println(rcvdPayload);
  }
  if((millis()-preMil) > intMil) { //5초가 지났는지 확인. 위에서 바운싱 이슈 땜에
    // read the state of the pushbutton value
    if (digitalRead(buttonPin)) {
      preMil = millis(); // 현재의 타이머 값을 preMil에 넣어 초기화
      sprintf(payload,"ESP32-Button:001 Pressed!!"); // %d 처럼 변수를 넣어 쓸 때 sprintf 사용
      if(testButton.publish(pTOPIC_NAME,payload) == 0) { //publish 수행
        Serial.print("Publish Message:");
        Serial.println(payload);
      }
      else{
        Serial.println("Publish failed");
      }
    }
  }
}
