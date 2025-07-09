#include <Arduino.h>
#include "segment_display.h"

SegmentDisplay::SegmentDisplay(const byte *digit_pins, const byte *segment_pins)
{
    this->digit_pins[0] = digit_pins[0];
    this->digit_pins[1] = digit_pins[1];
    this->digit_pins[2] = digit_pins[2];
    this->digit_pins[3] = digit_pins[3];

    this->segment_pins[0] = segment_pins[0];
    this->segment_pins[1] = segment_pins[1];
    this->segment_pins[2] = segment_pins[2];
    this->segment_pins[3] = segment_pins[3];
    this->segment_pins[4] = segment_pins[4];
    this->segment_pins[5] = segment_pins[5];
    this->segment_pins[6] = segment_pins[6];
}

void SegmentDisplay::displayNumber(const byte digit, const byte number)
{
}