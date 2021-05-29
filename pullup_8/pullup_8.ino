

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
}

void loop() {
  int value = digitalRead(8);
  Serial.println(String("Value: ") + value);
  delay(500);
}
