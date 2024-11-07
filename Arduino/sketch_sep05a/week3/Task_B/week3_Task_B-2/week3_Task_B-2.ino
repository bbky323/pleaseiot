// <Task 03-2>
// set pin numbers
const int ledPin = 16; // the number of the LED pin
// set varialbles
char serState = NULL;
void setup() {
Serial.begin(115200);
// initialize the LED pin as an output
pinMode(ledPin, OUTPUT);
}
void loop() {
// read the state of Serial port
if (Serial.available() > 0)
serState = Serial.read();
// check which keyboard 'h' or 'l' is pressed.
if (serState == 'h') {
// turn LED on
digitalWrite(ledPin, HIGH);
} else if (serState == 'l') {
// turn LED off
digitalWrite(ledPin, LOW);
}
delay(250);
}
