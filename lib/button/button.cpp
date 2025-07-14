#include <Arduino.h>
#include "button.h"

#define DEBOUNCE_MS 50

static Button BUTTONS[4];

void register_button(const byte id, const byte pin)
{
    BUTTONS[id] = Button(pin);
    pinMode(pin, INPUT_PULLUP);
}

void update_button_states()
{
    const long now = millis();

    for (Button &button : BUTTONS)
    {
        button.current_state = digitalRead(button.pin);

        // Debounce handling
        if (button.current_state != button.last_flicker_state)
        {
            button.last_flicker_state = button.current_state;
            button.last_debounce_time = now;
        }

        if (now - button.last_debounce_time > DEBOUNCE_MS)
        {
            // Setting button states after debounce conditions are true
            if (button.current_state == LOW && button.last_state == HIGH)
            {
                button.pressed = true;
                button.last_pressed_time = now;
            }
            else if (button.current_state == HIGH && button.last_state == LOW)
            {
                button.released = true;
                button.last_released_time = now;
            }
            else
            {
                button.pressed = false;
                button.released = false;
            }

            button.last_state = button.current_state;
        }
        }
}

bool is_button_just_pressed(const byte id)
{
    return BUTTONS[id].pressed;
}

bool is_button_just_released(const byte id)
{
    return BUTTONS[id].released;
}

bool is_button_held_for(const byte id, const unsigned long time_millis)
{
    const unsigned long now = millis();
    const Button button = BUTTONS[id];

    if (button.last_pressed_time == button.last_released_time)
        return false;

    const unsigned long time_held = now - button.last_debounce_time;
    // const bool test = !button.pressed && button.current_state == LOW && time_held >= time_millis;
    // Serial.println(test);
    // Serial.println(time_held);
    // Serial.println("vs: VVV");
    // Serial.println(time_millis);

    return !button.pressed && button.current_state == LOW && time_held >= time_millis;
}