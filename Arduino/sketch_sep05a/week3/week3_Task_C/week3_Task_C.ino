// set pin numbers
const int touchPin = 4;
const int buttonPin = 15;
const int ledPin = 16;
// change with your threshold value
const int threshold = 20;

// variable for storing the touch pin value
int touchValue;
int buttonState = 0;
int lastButtonState = 0;  // 이전 버튼 상태 저장
bool isPaused = false;    // Pause 상태를 저장

void setup() {
  Serial.begin(115200);
  delay(1000); // time to bring up serial monitor
  
  // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the touch value:
  touchValue = touchRead(touchPin);
  
  // 현재 버튼 상태 읽기
  buttonState = digitalRead(buttonPin);

  // 버튼이 눌러졌다가 떼어졌는지 확인 (상태가 LOW에서 HIGH로 변경될 때)
  if (buttonState == HIGH && lastButtonState == LOW) {
    // Pause/Resume 상태 토글
    isPaused = !isPaused;
    delay(50); // 버튼 바운스 방지
  }

  // 이전 버튼 상태 업데이트
  lastButtonState = buttonState;

  // Pause 상태가 아닐 때만 시리얼 출력
  if (!isPaused) {
    Serial.print(touchValue);
    Serial.println(buttonState);

    // check if the touchValue below the threshold
    // if it is, set ledPin to HIGH
    if (touchValue < threshold) {
      digitalWrite(ledPin, HIGH);
      Serial.println(" - LED on");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println(" - LED off");
    }
  }

  delay(500);
}
