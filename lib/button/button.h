#include <Arduino.h>

struct Button
{
    byte pin;
    byte current_state;
    byte last_state;
    byte last_flicker_state;
    long last_debounce_time;
    long last_pressed_time;
    long last_released_time;
    bool pressed;
    bool released;

    Button() : pin(0), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0), last_pressed_time(0), last_released_time(0), pressed(false), released(false) {}
    Button(byte pin) : pin(pin), current_state(LOW), last_state(LOW), last_flicker_state(LOW), last_debounce_time(0), last_pressed_time(0), last_released_time(0), pressed(false), released(false) {}
};

/**
 * Registers a button pin to be listened to.
 *
 * @param id ID of the button
 * @param pin Pin of the button on the microcontroller
 */
void register_button(const byte id, const byte pin);

/**
 * Updates whether a button is considered pressed, released, held, or none.
 */
void update_button_states();

/**
 * Checks if a button was just pressed.
 *
 * @param id ID of the button
 * @returns True if the state of the button is just pressed
 */
bool is_button_just_pressed(const byte id);

/**
 * Checks if a button was just released.
 * 
 * @param id ID of the button
 * @param True if the state of the button is just released
 */
bool is_button_just_released(const byte id);

/**
 * Checks if a button is held for a specified time (in milliseconds).
 * 
 * @param id ID of the button
 * @param time_millis Require time held in milliseconds
 * @param True if the state of the button is held
 */
bool is_button_held_for(const byte id, const unsigned long time_millis);