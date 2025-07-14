#include <Arduino.h>

/**
 * Handles displaying numbers and necessary symbols onto a 4-digit 7-segment display.
 */
class SegmentDisplay
{
private:
    byte digit_pins[4] = {0};
    byte segment_pins[7] = {0};

public:
    /**
     * Registers digit pins and segment pins.
     * @param digit_pins Array of digit pins (size 4)
     * @param segment_pins Array of segment pins (size 7)
     */
    void register_pins(const byte *digit_pins, const byte *segment_pins);
    
    /**
     * Displays a number on the specified digit.
     * @param digit The digit to display on (0-3)
     * @param number The number to display
     */
    void display_number(const byte digit, const byte number);

    /**
     * Displays the length mode symbol on D4.
     */
    void display_length_mode();

    /**
     * Displays the interval mode symbol on D4.
     */
    void display_interval_mode();
};