const int trigPin = 9;
const int echoPin = 10;
long duration, cm, inches;

const int buzzerPin = 6;
unsigned long lastBeep = 0;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();


  unsigned long now = millis();
  Serial.println(now);
  unsigned long beepIntervalMillis = cm * 20;
  Serial.println(beepIntervalMillis);
  if ((now - lastBeep) >= beepIntervalMillis) {
    tone(buzzerPin, 440, 50);
    lastBeep = now;
  }

  delay(100);
}
