#include <Arduino.h>

struct Button
{
    byte pin;
    byte current_state;
    byte last_state;
    byte last_flicker_state;
    long last_debounce_time;
    bool pressed;

    Button() : pin(0), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0), pressed(false) {}
    Button(byte pin) : pin(pin), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0), pressed(false) {}
};

/**
 * Registers the button pin to be listened to.
 *
 * @param id ID of the button
 * @param pin Pin of the button on the microcontroller
 */
void register_button(const byte id, const byte pin);

/**
 * Updates whether the button is considered pressed, released, or none.
 */
void update_button_states();

/**
 * Checks if the button was just pressed.
 *
 * @param id ID of the button
 * @returns True if the state of the button is just pressed
 */
bool is_button_just_pressed(const byte id);