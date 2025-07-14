#include <Arduino.h>

class SegmentDisplay
{
private:
    byte digit_pins[4] = {0};
    byte segment_pins[7] = {0};

public:
    void register_pins(const byte *digit_pins, const byte *segment_pins);
    // SegmentDisplay(const byte *digit_pins, const byte *segment_pins);
    void display_number(const byte digit, const byte number);
    void display_length_mode();
    void display_interval_mode();
};