int pause = 0;
int delta = 1;
volatile byte state = LOW;
int n = 0; // pwm value
void sw_pressed() {
  state = HIGH;
  Serial.println("Interrupt..");
}
void setup()
{
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2),
  sw_pressed, RISING);
}
void loop()
{
  if (pause == 0) { // pause가 0이 아닐때만 진행
    analogWrite(11, n);
    n += delta;
    if ( n == 255 || n == 0 ) delta *= -1;
    Serial.println(n);
  }
  if (state == HIGH) {
    pause = pause ? 0 : 1;
    Serial.print("pause= ");
    Serial.println(pause);
    state = LOW;
  }
}
