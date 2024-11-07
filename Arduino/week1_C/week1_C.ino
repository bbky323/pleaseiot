/* Blink */
// ledPin refers to ESP32 GPIO 23
const int ledPin = 23;
const int ledPin2 = 22;
int cnt=0;
int i=1000;
// the setup function runs once when you press reset or power the board
void setup() {
// initialize digital pin ledPin as an output.
pinMode(ledPin, OUTPUT);
pinMode(ledPin2, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
digitalWrite(ledPin, HIGH);
digitalWrite(ledPin2, LOW);// turn the LED on (HIGH is the voltage level)
delay(i); // wait for a second
cnt+=i;
digitalWrite(ledPin, LOW);
digitalWrite(ledPin2, HIGH);
// turn the LED off by making the voltage LOW
delay(i); // wait for a second
cnt+=i;

if(cnt==2000){
  if(i!=125){
    i/=2;
  }
  else{
    i=1000;
  }
  cnt=0;
}

}
