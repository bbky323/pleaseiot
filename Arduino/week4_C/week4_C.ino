// Setting PWM properties
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 23;
const int duty = 128;

// Notes frequencies (C4, D4, E4, F4, G4, A4, B4, C5)
int nFrq[] = {262, 294, 330, 349, 392, 440, 494, 523};

// "곰 세 마리" melody (C, G, A, G, C, G, C, D, E, F, G)
int melody[] = {0, 0, 0, 0, 0, 2, 4, 4, 2, 0, -1, 4, 4, 2, 4, 4, -1, 0, 0, 0, -1,
    4, 4, 2, 0, 4, 4, 4, -1, 4, 4, 2, 0, 4, 4, 4, -1,
    4, 4, 2, -1, 0, 4, 0, 4, 2, 1, 0};  // 음계를 숫자로 표현
int noteDurations[] = {500, 250, 250, 500, 500, 500, 250, 250, 500, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500,
500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}; // 음의 길이

void setup() {
  // Configure PWM channel
  ledcSetup(ledChannel, 5000, resolution);
  ledcAttachPin(buzPin, ledChannel);
}

void playTone(int frequency, int duration) {
  ledcWriteTone(ledChannel, frequency); // Set the frequency
  delay(duration);  // Hold the tone for the duration
  ledcWriteTone(ledChannel, 0);  // Stop the tone
}

void loop() {
  for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    int noteIndex = melody[i];
    int noteDuration = noteDurations[i];
    playTone(nFrq[noteIndex], noteDuration);
    
    delay(100); // Short pause between notes
  }

  delay(1000); // Pause before repeating the melody
}
