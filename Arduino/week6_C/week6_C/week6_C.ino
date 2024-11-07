#include <EEPROM.h>

// PWM 설정
const int ledChannel = 0;
const int resolution = 10;
const int buzPin = 23;
const int duty = 128;
const int maxSize = 100;  // 저장할 수 있는 최대 노트-지속시간 쌍의 개수

// 변수
int vNote=0, vDur=0;
const int dDur = 250; // 기본 지속시간
bool storingMusic = false;
int noteCount = 0;
const int signature1 = 0xAA;
const int signature2 = 0x55;

// 음과 지속 시간
int nFrq[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 0};
int nDur[] = { 2000, 1500, 1000, 750, 500, 375, 250 };


void clearEEPROM() {
  // EEPROM의 모든 주소에 0을 기록
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit(); // 변경 사항을 저장
  Serial.println("EEPROM memory has been cleared.");
}

void playNote(int note, int dur) {
  if (note == 13) {
    //ledcSetup(ledChannel, 0, resolution);
    ledcWrite(ledChannel, 0);
  }
  else {
    ledcSetup(ledChannel, nFrq[note], resolution);
    ledcWrite(ledChannel, duty);
  }
  Serial.println(String(note)+","+String(dur));
  delay(nDur[dur]);
  ledcWrite(ledChannel, 0);
}

// 노트 및 지속시간 EEPROM에 저장
void storeNoteInEEPROM(int note, int dur) {
  if (noteCount < maxSize) {
    EEPROM.write(3 + noteCount * 2, note);  // EEPROM에 노트 저장
    EEPROM.write(3 + noteCount * 2 + 1, dur);  // EEPROM에 지속시간 저장
    noteCount++;
    EEPROM.write(2, noteCount);  // EEPROM에 노트-지속시간 쌍 개수 저장
    EEPROM.commit();
  } else {
    Serial.println("EEPROM is full.");
  }
}

// EEPROM에서 노트 및 지속시간 읽기
void playMusicFromEEPROM() {
  int savedNoteCount = EEPROM.read(2);
  for (int i = 0; i < savedNoteCount; i++) {
    int note = EEPROM.read(3 + i * 2);
    int dur = EEPROM.read(3 + i * 2 + 1);
    playNote(note, dur);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);  // EEPROM 초기화

  // attach the channel to the GPIOs
  ledcAttachPin(buzPin, ledChannel);

  //clearEEPROM();

  // EEPROM에서 유효성 검사 후 음악 재생
  if (EEPROM.read(0) == signature1 && EEPROM.read(1) == signature2) {
    Serial.println("Valid music found in EEPROM, playing...");
    playMusicFromEEPROM();
  } else {
    Serial.println("No valid music in EEPROM.");
  }
}

void loop(){
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == '<') {
      storingMusic = true;
      noteCount = 0; // 새로 저장 시작
      EEPROM.write(0, signature1); // 유효성 서명 저장
      EEPROM.write(1, signature2);
      EEPROM.commit();
      Serial.println("Start storing music.");
    } else if (ch == '>') {
      storingMusic = false;
      Serial.println("Finished storing music.");
    } else {
      vNote = ch;
      if (Serial.available() > 0) {
        vDur = Serial.read();
        if (vDur <= '6' && vDur >= '0')
          vDur -= '0';
        else vDur = dDur;

        if (vNote <= '9' && vNote >= '0')
          vNote -= '0';
        else if (vNote <= 'c' && vNote >= 'a')
          vNote = vNote - 'a' + 10;
        else
          vNote = 13; // 쉼표 처리

        playNote(vNote, vDur);

        if (storingMusic) {
          storeNoteInEEPROM(vNote, vDur);
        }
      }
    }
  }
}
