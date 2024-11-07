#define timeSeconds 20

const int led1=5;
const int led2=17;
const int led3=16;
const int motionSensor=36;

unsigned long now=millis();
unsigned long lastTrigger[]={0, 0, 0};
boolean startTimer[]= {false, false, false};
boolean flag[] = {false, false, false};
int i = 0;
int a[] = {5, 17, 16};

void IRAM_ATTR detectsMovement() {
  flag[i]=true;
  digitalWrite(a[i], HIGH);
  startTimer[i]=true;
  lastTrigger[i]=millis();
}
void setup() {
  Serial.begin(115200);
  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  pinMode(led3, OUTPUT);
  digitalWrite(led3, LOW);
  // put your setup code here, to run once:

}

void loop() {
  now=millis();
  if (flag[i]) {
    Serial.println("MOTION (" + String(i) + ") DETECTED!!!");
    flag[i]=false;
    i=(i+1)%3;
  }
  for (int j = 0; j < 3; j++) {
    if (startTimer[j] && (now - lastTrigger[j] > (timeSeconds * 1000))) {
      Serial.println("Motion (" + String(j) + ") stopped...");
      digitalWrite(a[j], LOW);
      startTimer[j]=false;
    
    }
  }
  // put your main code here, to run repeatedly:

}
