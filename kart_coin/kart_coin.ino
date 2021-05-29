#include <LiquidCrystal.h>

const int buttonPin = 7;
const int buzzerPin = 9;
int buttonState = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


const int COIN_TRACK_LENGTH = 15;
int coin_track[COIN_TRACK_LENGTH];
int kart_position = 1;
unsigned int score = 0;

const unsigned long INITIAL_COIN_UPDATE_DELAY = 400;
const unsigned long SPEED_UP_DELAY = 1000;
const unsigned long SPEED_UP_AMOUNT = 10;
unsigned long last_speed_up = 0;

unsigned long coin_update_delay = INITIAL_COIN_UPDATE_DELAY;
unsigned long last_coin_update = 0;
unsigned long game_duration_ms = 30 * 1000;

int last_button_state = HIGH; // HIGH == Not pressed.
unsigned long last_kart_move = 0;
unsigned long move_delay_ms = 100; 

bool redraw_needed = false;

bool game_over = false;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("KART COIN");
  delay(500);

  fill_array(coin_track, COIN_TRACK_LENGTH, -1);

  // Comment this if you prefer the game to be deterministic.
  randomSeed(analogRead(0));
}

void loop() {
  game_update();
  game_draw();
  if (game_over) {
    lcd.clear();
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print(String("Score: ") + String(score, DEC));
    delay(60 * 60 * 1000);
  }
}

void fill_array(int arr[], int arr_size, int value) {
  for (int i = 0; i < arr_size; i++) {
    arr[i] = value;
  }
}

bool check_array_all_equal(int arr[], int arr_size, int value) {
  for (int i = 0; i < arr_size; i++) {
    if (arr[i] != value) {
      return false;
    }
  }
  return true;
}

void game_update() {
  unsigned long now = millis();

  bool time_over = now > game_duration_ms;

  int button_state = digitalRead(buttonPin);
  unsigned long time_since_last_move = now - last_kart_move;
  if ((last_button_state == HIGH) && (button_state == LOW) && (time_since_last_move > move_delay_ms)) {
    if (kart_position == 0) {
      kart_position = 1;
    } else {
      kart_position = 0;
    }
    last_kart_move = now;
    redraw_needed = true;
  }
  last_button_state = button_state;

  // Speed up.
  unsigned long time_since_last_speed_up = now - last_speed_up;
  if (time_since_last_speed_up > SPEED_UP_DELAY) {
    coin_update_delay -= SPEED_UP_AMOUNT;
    last_speed_up = now;
  }

  unsigned long time_since_last_coin_update = now - last_coin_update;
  if (time_since_last_coin_update > coin_update_delay) {
    // Grab the last coin.
    if (coin_track[COIN_TRACK_LENGTH - 1] == kart_position) {
      score += 1;
      tone(buzzerPin, 1500, 50);
    }
    Serial.println(String("Score: ") + String(score, DEC));
  
    // Push the coins to the right.
    for (int i = COIN_TRACK_LENGTH - 1; i > 0 ; i--) {
      coin_track[i] = coin_track[i - 1];
    }
  
    // Add new coin.
    int new_coin_pos = random(-2, 2);
    if (new_coin_pos < 0) {
      new_coin_pos = -1;
    }
    if (time_over) {
      new_coin_pos = -1;
    }
    Serial.print("New coin position: ");
    Serial.println(new_coin_pos);
    coin_track[0] = new_coin_pos;

    last_coin_update = now;
    redraw_needed = true;
  }

  // Check for game over.
  bool no_coins = check_array_all_equal(coin_track, COIN_TRACK_LENGTH, -1);
  if (time_over && no_coins) {
    game_over = true;
  }
}

void game_draw() {
  if (!redraw_needed) {
    return;
  }
  lcd.clear();
  for (int i = 0; i < COIN_TRACK_LENGTH; i++) {
    if (coin_track[i] != -1) {
      lcd.setCursor(i, coin_track[i]);
      lcd.write('-');
    }
  }
  lcd.setCursor(15, kart_position);
  lcd.write('<');
  redraw_needed = false;
}
