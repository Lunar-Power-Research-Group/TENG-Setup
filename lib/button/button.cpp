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

        if (button.current_state != button.last_flicker_state)
        {
            button.last_flicker_state = button.current_state;
            button.last_debounce_time = now;
        }

        if (now - button.last_debounce_time > DEBOUNCE_MS)
            button.last_state = button.current_state;
    }
}

bool is_button_just_pressed(const byte id)
{
    const long now = millis();
    const Button button = BUTTONS[id];

    return digitalRead(button.pin) == LOW && button.last_state == HIGH && now - button.last_debounce_time > DEBOUNCE_MS;
}
