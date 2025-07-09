#include <Arduino.h>

class SegmentDisplay
{
private:
    byte digit_pins[4];
    byte segment_pins[7];
public:
    SegmentDisplay(const byte *digit_pins, const byte *segment_pins);
    void displayNumber(const byte digit, const byte number);
    void displayLengthMode();
    void displayIntervalMode();
};