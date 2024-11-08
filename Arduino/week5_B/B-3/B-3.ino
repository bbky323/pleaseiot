#define timeSeconds 3
// Set GPIOs for LED and PIR Motion Sensor
const int led = 16;
const int motionSensor = 36;
// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean flag= false;
// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
flag=true;
digitalWrite(led, HIGH);
startTimer = true;
lastTrigger = millis();
}
void setup() {
// Serial port for debugging purposes
Serial.begin(115200);
// PIR Motion Sensor mode INPUT_PULLUP
pinMode(motionSensor, INPUT_PULLUP);
// Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
// Set LED to LOW
pinMode(led, OUTPUT);
digitalWrite(led, LOW);
}
void loop() {
// Current time
now = millis();
if (flag) Serial.println("MOTION DETECTED!!!");
// Turn off the LED after the number of seconds defined in the timeSeconds variable
if (startTimer && (now - lastTrigger > (timeSeconds*1000))) {
  Serial.println("Motion stopped...");
digitalWrite(led, LOW);
startTimer = false;
}
flag=false;
}
