const int buzzer = 9; //buzzer to arduino pin 9


void setup(){
 
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

}

void loop(){
 
  tone(buzzer, 440, 1000); // A_5
  delay(1000);
  tone(buzzer, 494, 1000); // B_5
  delay(1000);
  tone(buzzer, 523, 1000); // C_5
  delay(1000);
  tone(buzzer, 587, 1000); // D_5
  delay(1000);
  tone(buzzer, 659, 1000); // E_5
  delay(1000);
  tone(buzzer, 698, 1000); // F_5
  delay(1000);
  tone(buzzer, 784, 1000); // G_5
  delay(1000);

  delay(1 * 60 * 60 * 1000);
}
