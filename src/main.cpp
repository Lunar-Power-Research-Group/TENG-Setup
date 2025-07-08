#include <Arduino.h>
#include <Servo.h>
#include "utils.h"

#define STATUS_LED LED_BUILTIN
#define SERVO_PIN 0
#define START_BUTTON 2
#define INCREMENT_BUTTON 4
#define DECREMENT_BUTTON 7
#define CHANGE_SETTING_BUTTON 8

#define GEAR_PITCH 30 // millimeters

struct Setting
{
  enum Value
  {
    GAP = 0,
    INTERVAL,
    N_ITEMS
  };

  static const String ToStr(const Setting::Value &value)
  {
    switch (value)
    {
    case GAP:
      return "Gap";
    case INTERNAL:
      return "Interval";
    default:
      return "Unknown";
    }
  }
};

Servo servo;

bool active = false;
Setting::Value setting_mode = Setting::GAP;

// Settings
byte height = 10;
short height_angle = 0;
short separation_interval = 0;
short write_delay = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(STATUS_LED, OUTPUT);
  pinMode(INCREMENT_BUTTON, INPUT_PULLUP);
  pinMode(DECREMENT_BUTTON, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(CHANGE_SETTING_BUTTON, INPUT_PULLUP);

  // Attach servo
  servo.attach(SERVO_PIN);

  // Reset servo to contact state
  servo.write(0);
}

static bool is_button_pressed(const int pin)
{
  return !digitalRead(pin);
}

static short get_write_delay(const short interval, const short height_angle)
{
  return (interval / 2) / height_angle;
}

static short get_height_angle(const short height)
{
  return 180 * height / (PI * GEAR_PITCH);
}

void handle_inputs()
{
  // Toggle active state
  if (is_button_pressed(START_BUTTON))
  {
    if (active)
    {
      active = false;
      digitalWrite(STATUS_LED, LOW);
    }
    else
    {
      active = true;
      digitalWrite(STATUS_LED, HIGH);
    }
  }

  // Setup configuration
  if (is_button_pressed(CHANGE_SETTING_BUTTON))
  {
    setting_mode = static_cast<Setting::Value>((setting_mode + 1) % Setting::N_ITEMS);
  }

  if (is_button_pressed(INCREMENT_BUTTON))
  {
    switch (setting_mode)
    {
    case Setting::GAP:
    {
      height_angle = get_height_angle(++height);
      write_delay = get_write_delay(separation_interval, height_angle);

      Serial.println("Height:");
      Serial.println(height);
      break;
    }
    case Setting::INTERVAL:
    {
      write_delay = get_write_delay(++separation_interval, height_angle);

      Serial.println("Interval:");
      Serial.println(separation_interval);
      break;
    }
    }
  }
  else if (is_button_pressed(DECREMENT_BUTTON))
  {
    switch (setting_mode)
    {
    case Setting::GAP:
    {
      height_angle = get_height_angle(--height);
      write_delay = get_write_delay(separation_interval, height_angle);

      Serial.println("Height:");
      Serial.println(height);
      break;
    }
    case Setting::INTERVAL:
    {
      write_delay = get_write_delay(--separation_interval, height_angle);

      Serial.println("Interval:");
      Serial.println(separation_interval);
      break;
    }
    }
  }
}

void loop()
{
  handle_inputs();

  // Contact and separation functionality
  if (active)
  {
    const short current_height_angle = height_angle;
    const short current_write_delay = write_delay;

    // Separation
    for (short angle = 0; angle <= current_height_angle; angle++)
    {
      servo.write(angle);
      delay(current_write_delay);
    }

    // Contact
    for (short angle = current_height_angle; angle >= 0; angle--)
    {
      servo.write(angle);
      delay(current_write_delay);
    }
  }
}