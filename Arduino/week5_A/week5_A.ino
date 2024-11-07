const int trigPin = 22;
const int echoPin = 23;
const int buzPin = 21;  // 버저가 연결된 핀
const int ledChannel = 0;  // PWM 채널
const int resolution = 8;  // PWM 해상도
const int minFreq = 100;  // 최소 주파수 (거리가 멀 때 낮은 음)
const int maxFreq = 2000;  // 최대 주파수 (거리가 가까울 때 높은 음)

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // PWM 설정
  ledcSetup(ledChannel, minFreq, resolution);
  ledcAttachPin(buzPin, ledChannel);
}

void loop()
{
  long duration, distance;
  
  // Triggering by 10us pulse
  digitalWrite(trigPin, LOW); // trig low for 2us
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // trig high for 10us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // getting duration for echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  // sound speed = 340 m/s = 34/1000 cm/us
  // distance = duration * 34/1000 * 1/2
  distance = duration * 17 / 1000;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Calculate frequency based on distance
  int frequency = map(distance, 2, 400, maxFreq, minFreq);  // 거리에 따라 주파수를 매핑 (최소 2cm ~ 최대 400cm)
  frequency = constrain(frequency, minFreq, maxFreq);  // 주파수 제한
  
  // Set buzzer frequency
  ledcWriteTone(ledChannel, frequency);
  
  delay(100);
}
