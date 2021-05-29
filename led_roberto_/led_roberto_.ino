
const int pino_led = 8;
const int led_delay = 1;

void setup() {
  pinMode(pino_led, OUTPUT);
}

void loop() {
  tone(pino_led, 329, 500);
  delay(500);


//  delay(100);
}
