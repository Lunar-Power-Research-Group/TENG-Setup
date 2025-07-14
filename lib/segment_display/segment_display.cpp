#include <Arduino.h>
#include "segment_display.h"

#define SEGMENT_A 0
#define SEGMENT_B 1
#define SEGMENT_C 2
#define SEGMENT_D 3
#define SEGMENT_E 4
#define SEGMENT_F 5
#define SEGMENT_G 6

static bool SEGMENTS[10][7] = {
    { 1, 1, 1, 1, 1, 1, 0 },    // 0
    { 0, 1, 1, 0, 0, 0, 0 },    // 1
    { 1, 1, 0, 1, 1, 0, 1 },    // 2
    { 1, 1, 1, 1, 0, 0, 1 },    // 3
    { 0, 1, 1, 0, 0, 1, 1 },    // 4
    { 1, 0, 1, 1, 0, 1, 1 },    // 5
    { 1, 0, 1, 1, 1, 1, 1 },    // 6
    { 1, 1, 1, 0, 0, 0, 0 },    // 7
    { 1, 1, 1, 1, 1, 1, 1 },    // 8
    { 1, 1, 1, 1, 0, 1, 1 }     // 9
};

void SegmentDisplay::register_pins(const byte *digit_pins, const byte *segment_pins)
{
    memcpy(this->digit_pins, digit_pins, 4 * sizeof(byte));
    memcpy(this->segment_pins, segment_pins, 7 * sizeof(byte));

    for (int i = 0; i < 4; i++)
        pinMode(digit_pins[i], OUTPUT);

    for (int i = 0; i < 7; i++)
        pinMode(segment_pins[i], OUTPUT);
}

static void select_digit(const byte digit, const byte *digit_pins)
{
    for (int i = 0; i < 4; i++)
        digitalWrite(digit_pins[i], digit == i);
}

void SegmentDisplay::display_number(const byte digit, const byte number)
{
    select_digit(digit, digit_pins);

    const byte digit_num = number % 10;
    const bool *segments = SEGMENTS[digit_num];

    for (int segment_index = 0; segment_index < 7; segment_index++)
        digitalWrite(segment_pins[segment_index], !segments[segment_index]);
}

void SegmentDisplay::display_length_mode()
{
    select_digit(3, digit_pins);

    digitalWrite(segment_pins[SEGMENT_A], HIGH);
    digitalWrite(segment_pins[SEGMENT_B], HIGH);
    digitalWrite(segment_pins[SEGMENT_C], HIGH);
    digitalWrite(segment_pins[SEGMENT_D], LOW);
    digitalWrite(segment_pins[SEGMENT_E], LOW);
    digitalWrite(segment_pins[SEGMENT_F], LOW);
    digitalWrite(segment_pins[SEGMENT_G], HIGH);
}

void SegmentDisplay::display_interval_mode()
{
    select_digit(3, digit_pins);

    digitalWrite(segment_pins[SEGMENT_A], HIGH);
    digitalWrite(segment_pins[SEGMENT_B], HIGH);
    digitalWrite(segment_pins[SEGMENT_C], HIGH);
    digitalWrite(segment_pins[SEGMENT_D], LOW);
    digitalWrite(segment_pins[SEGMENT_E], LOW);
    digitalWrite(segment_pins[SEGMENT_F], LOW);
    digitalWrite(segment_pins[SEGMENT_G], LOW);
}