#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                      // SPI hardware interface
//MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); // Arbitrary pins

#define N_COLS 8 * MAX_DEVICES

byte rain_arr[N_COLS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void resetMatrix(void)
{
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY/64);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
}

byte binary_choice(byte a, byte b, int proba_res, int proba_thresh) {
    if (random(proba_res) < proba_thresh) {
        return a;
    } else {
        return b;
    }
}

byte new_rain_column() {
    byte new_row = 0b00000000;
    new_row |= binary_choice(0b00000000, 0b10000000, 8, 7);
    new_row |= binary_choice(0b00000000, 0b01000000, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00100000, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00010000, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00001000, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00000100, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00000010, 8, 7);
    new_row |= binary_choice(0b00000000, 0b00000001, 8, 7);
    return new_row;
}

void setup() {
    mx.begin();
    resetMatrix();
}

void loop() {
    byte swap_column = new_rain_column();
    byte old_column;
    for (size_t i = 0; i < N_COLS; i++)
    {
        old_column = rain_arr[i];
        rain_arr[i] = swap_column;
        swap_column = old_column;
    }

    mx.clear();
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (uint8_t i = 0; i < N_COLS; i++)
    {
        mx.setColumn(i, rain_arr[i]);
    }
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    delay(100);
}
