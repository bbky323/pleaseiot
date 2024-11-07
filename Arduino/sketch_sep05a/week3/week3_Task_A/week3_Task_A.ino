// <Task 03-4>
// set pin numbers
const int touchPin = 4;
const int buttonPin = 15;
const int ledPin = 16;
// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value
int touchValue;
int buttonState = 0;
void setup() {
Serial.begin(115200);
delay(1000); // time to bring up serial monitor
// initialize the LED pin as an output:
pinMode(buttonPin, INPUT);
pinMode (ledPin, OUTPUT);
}
void loop() {
// read the touch value:
touchValue = touchRead(touchPin);
Serial.print(touchValue);
buttonState = digitalRead(buttonPin);
Serial.println(buttonState);
// check if the touchValue below the
// threshold
// if it is, set ledPin to HIGH
if(touchValue < threshold || buttonState == HIGH) {
// turn LED on
digitalWrite(ledPin, HIGH);
Serial.println(" - LED on");
digitalWrite(ledPin, HIGH);
}
else {
// turn LED off
digitalWrite(ledPin, LOW);
Serial.println(" - LED off");
digitalWrite(ledPin, LOW);
}
delay(500);
}
