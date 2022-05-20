#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 8
#define MAX_RGB_VALUE 50

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

CHSV new_random_chsv() {
    return CHSV(
        random(255),
        255,
        50
    );
}

CHSV new_random_fire() {
    return CHSV(
        random(0, 42),
        255,//random(0, 200),
        random(125, 150) //random(50)
    );
}

CRGB new_random_crgb() {
    return CRGB(
        random(MAX_RGB_VALUE),
        random(MAX_RGB_VALUE),
        random(MAX_RGB_VALUE)
    );
}


void loop() {
    CRGB temp_color = new_random_fire();
    CRGB old_color;

    for (int i = 0; i <= NUM_LEDS - 1; i++) {
        old_color = leds[i];
        leds[i] = temp_color;
        temp_color = old_color;
        FastLED.show();
        delay(40);
    }
}