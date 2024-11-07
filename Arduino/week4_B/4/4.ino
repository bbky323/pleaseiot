// make tones by using pwm
// freq -> pitch
// duty -> volume (?)
// setting PWM properties
//const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 23;
const int duty = 128;
// variables
int sVal;
// notes
//enum Notes {C3=0, CS3, D3, DS3, E3, F3};
int nFrq[] = {/*131, 139, 147, 156, 165, 175*/
262, 277, 294, 311, 330, 349};

void playNote(int note, int dur) {
  if(note >= 0 && note < 6) { // 유효한 note 범위 확인
    ledcWriteTone(ledChannel, nFrq[note]); // 주파수 설정
    ledcWrite(ledChannel, duty); // 볼륨 설정
    Serial.println(note); // 디버깅용 출력
    delay(dur); // 음을 재생할 시간
    ledcWriteTone(ledChannel, 0); // 소리를 끔
  } else {
    Serial.println("Invalid note!"); // 유효하지 않은 입력값 처리
  }
}
void setup() {
Serial.begin(115200);
ledcAttachPin(buzPin, ledChannel);
}
void loop(){
if (Serial.available() > 0) {
sVal = Serial.read();
playNote(sVal-0x30, 250);
}
}
