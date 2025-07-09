#include <Arduino.h>

struct Button
{
    byte pin;
    byte current_state;
    byte last_state;
    byte last_flicker_state;
    long last_debounce_time;

    Button() : pin(0), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0) {}
    Button(byte pin) : pin(pin), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0) {}
};

void register_button(const byte id, const byte pin);
void update_button_states();
bool is_button_just_pressed(const byte id);