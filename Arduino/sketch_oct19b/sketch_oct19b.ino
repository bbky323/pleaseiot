#include <Wire.h>

int LED = 13; // LED Pin
byte MyID = 1; // Slave ID

byte receivedData = 0; // Received data
volatile byte sendBlinkNo = 0; // Blink number to send
volatile boolean requestEvent = false;

void setup() {
  Wire.begin(MyID); // Slave id
  Wire.onReceive(receiveEvent); // receive handler
  Wire.onRequest(sendEvent); // transmit handler
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    byte c = Serial.read() - '0';
    if (c > 0 && c < 10) {
      sendBlinkNo = c;
      Serial.print("Send ");
      Serial.print(c);
      Serial.println(" to Master");
    }
  }

  if (requestEvent) {
    requestEvent = false;
    requestFromMaster();
  }
}

void receiveEvent(int bytes) { // read from master
  receivedData = Wire.read();
  requestEvent = true;
}

void sendEvent() {
  Wire.write(sendBlinkNo);
  sendBlinkNo = 0;
}

void requestFromMaster() {
  for (int i = 0; i < receivedData; i++) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}
