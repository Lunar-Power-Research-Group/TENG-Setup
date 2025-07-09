#include <Arduino.h>
#include <Servo.h>
#include "button.h"
#include "utils.h"

#define STATUS_LED LED_BUILTIN
#define SERVO_PIN 12
#define START_BUTTON_PIN 2
#define INCREMENT_BUTTON_PIN 4
#define DECREMENT_BUTTON_PIN 7
#define CHANGE_SETTING_BUTTON_PIN 8

#define START_BUTTON 0
#define INCREMENT_BUTTON 1
#define DECREMENT_BUTTON 2
#define CHANGE_SETTING_BUTTON 3

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
    case INTERVAL:
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
short separation_interval = 1000;
short write_delay = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(STATUS_LED, OUTPUT);

  // Register buttons
  register_button(START_BUTTON, START_BUTTON_PIN);
  register_button(INCREMENT_BUTTON, INCREMENT_BUTTON_PIN);
  register_button(DECREMENT_BUTTON, DECREMENT_BUTTON_PIN);
  register_button(CHANGE_SETTING_BUTTON, CHANGE_SETTING_BUTTON_PIN);

  // Attach servo
  servo.attach(SERVO_PIN);

  // Reset servo to contact state
  servo.write(180);
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
  if (is_button_just_pressed(START_BUTTON))
  {
    Serial.println("Pressed start button");
    if (active)
    {
      active = false;
      digitalWrite(STATUS_LED, LOW);
      Serial.println("Stop");
    }
    else
    {
      active = true;
      digitalWrite(STATUS_LED, HIGH);
      Serial.println("Start");
    }
  }

  // Setup configuration
  if (is_button_just_pressed(CHANGE_SETTING_BUTTON))
  {
    setting_mode = static_cast<Setting::Value>((setting_mode + 1) % Setting::N_ITEMS);
  }

  if (is_button_just_pressed(INCREMENT_BUTTON))
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
      separation_interval += 100;
      write_delay = get_write_delay(separation_interval, height_angle);

      Serial.println("Interval:");
      Serial.println(separation_interval);
      break;
    }
    default:
      break;
    }
  }
  else if (is_button_just_pressed(DECREMENT_BUTTON))
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
      separation_interval -= 100;
      write_delay = get_write_delay(separation_interval, height_angle);

      Serial.println("Interval:");
      Serial.println(separation_interval);
      break;
    }
    default:
      break;
    }
  }
}

bool rising = false;
long last_dir_change = 0;
short angle = 180;

void handle_servo_tick()
{
  if (rising)
  {
    if (angle <= 180 - height_angle)
    {
      rising = false;
      angle = 180 - height_angle;
      return;
    }

    angle--;
  }
  else
  {
    if (angle >= 180)
    {
      rising = true;
      angle = 180;
      return;
    }

    angle++;
  }

  servo.write(angle);
}

void loop()
{
  handle_inputs();
  update_button_states();

  // Contact and separation functionality
  if (active)
  {
    long now = millis();

    if (now - last_dir_change >= write_delay)
    {
      handle_servo_tick();
      last_dir_change = now;
    }
  }
}