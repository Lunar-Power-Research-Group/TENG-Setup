#include <Arduino.h>
#include "segment_display.h"

#define SEGMENT_A 0
#define SEGMENT_B 1
#define SEGMENT_C 2
#define SEGMENT_D 3
#define SEGMENT_E 4
#define SEGMENT_F 5
#define SEGMENT_G 6

void SegmentDisplay::register_pins(const byte *digit_pins, const byte *segment_pins)
{
    memcpy(this->digit_pins, digit_pins, 4 * sizeof(byte));
    memcpy(this->segment_pins, segment_pins, 7 * sizeof(byte));

    for (int i = 0; i < 4; i++)
    {
        byte pin = digit_pins[i];
        pinMode(pin, OUTPUT);
    }

    for (int i = 0; i < 7; i++)
    {
        byte pin = segment_pins[i];
        pinMode(pin, OUTPUT);
    }
}

static void select_digit(const byte digit, const byte *digit_pins)
{
    // digitalWrite(digit_pins[0], LOW);
    // digitalWrite(digit_pins[1], LOW);
    // digitalWrite(digit_pins[2], LOW);
    // digitalWrite(digit_pins[3], LOW);

    for (int i = 0; i < 4; i++)
    {
        bool state = digit == i ? HIGH : LOW;
        digitalWrite(digit_pins[i], state);
    }
}

void SegmentDisplay::display_number(const byte digit, const byte number)
{
    const byte digit_num = number % 10;
    select_digit(digit_num, digit_pins);

    byte a = (number >> 3) & 1;
    byte b = (number >> 2) & 1;
    byte c = (number >> 1) & 1;
    byte d = number & 1;

    bool seg_a = (~a & c) | (b & c) | (a & ~d) | (~b & ~d) | (~a & b & d) | (a & ~b & ~c);
    bool seg_b = (~a & ~b) | (~b & ~d) | (~b & ~c) | (~a & c & ~d) | (a & c & d) | (a & ~c & d);
    bool seg_c = (~a & ~c) | (~a & d) | (~c & d) | (~a & b) | (a & ~b);
    bool seg_d = (a & ~c) | (~a & ~b & ~d) | (b & c & d) | (b & ~c & d) | (b & c & ~d);
    bool seg_e = (~b & ~d) | (c & ~d) | (a & b) | (a & c);
    bool seg_f = (~c & ~d) | (b & ~d) | (a & c) | (a & ~b) | (~a & b & ~c);
    bool seg_g = (c & ~d) | (a & ~b) | (a & d) | (a & c) | (~b & c) | (~a & b & ~c);

    digitalWrite(segment_pins[0], !seg_a);
    digitalWrite(segment_pins[1], !seg_b);
    digitalWrite(segment_pins[2], !seg_c);
    digitalWrite(segment_pins[3], !seg_d);
    digitalWrite(segment_pins[4], !seg_e);
    digitalWrite(segment_pins[5], !seg_f);
    digitalWrite(segment_pins[6], !seg_g);
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