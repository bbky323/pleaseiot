//10.17(화) slave 0 실습코드
#include <SPI.h>

volatile int iflag = 0;
volatile byte n;
volatile byte ACK;

void setup(void) {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  SPCR |= _BV(SPE);
  SPCR &= ~_BV(MSTR);
  SPCR |= _BV(SPIE);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

// SPI 인터럽트 루틴
ISR(SPI_STC_vect) {
  iflag += 1;
  if ( SPDR > '0' && SPDR <= '9') {
    n = SPDR;
    ACK = n + 1;
    n = n - '0';
    SPDR = ACK;
  }
}

void loop(void) {
  if (iflag) {
    Serial.println("help");
    Serial.println(iflag);
    iflag = 0;
    Serial.print("No: ");
    Serial.println(n);
    for (int i = 0; i < n ; i++) {
      digitalWrite(7, HIGH);
      delay(500);
      digitalWrite(7, LOW);
      delay(500);
      Serial.print("help:");
      Serial.println(n);
    }
  }
}
