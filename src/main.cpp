#include <Arduino.h>
#include <Servo.h>
#include <MultiFuncShield.h>

#define STATUS_LED LED_BUILTIN
#define SERVO_PIN 5
#define INCREMENT_BUTTON_PIN A1
#define DECREMENT_BUTTON_PIN A2
#define CHANGE_SETTING_BUTTON_PIN A3

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
bool queue_end = false;
Setting::Value setting_mode = Setting::GAP;

// Settings
byte height = 10;
short height_angle = 0;
short separation_interval = 1000;
short write_delay = 0;

// Servo
bool rising = true;
long last_dir_change = 0;
short angle = 180;

void update_segment_display()
{
  String prefix;
  float value;

  switch (setting_mode)
  {
  case Setting::GAP:
  {
    prefix = "L";
    value = height;
    break;
  }
  case Setting::INTERVAL:
  {
    prefix = "t";
    value = separation_interval / 1000.0;
    break;
  }
  default:
  {
    MFS.write("----", 0);
    return;
  }
  }

  String display = prefix + ' ' + value;
  MFS.write(display.c_str(), 0);
}

void setup()
{
  Serial.begin(9600);

  pinMode(STATUS_LED, OUTPUT);

  // Register 7-segment display
  MFS.initialize();
  MFS.setDisplayBrightness(1);
  update_segment_display();

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

void end_if_queued()
{
  if (queue_end)
  {
    queue_end = false;
    active = false;
    update_segment_display();
    digitalWrite(STATUS_LED, LOW);
  }
}

static bool is_button_short_released(const byte btn, const uint8_t id)
{
  return btn == (id | BUTTON_SHORT_RELEASE_IND);
}

static bool is_button_long_released(const byte btn, const uint8_t id)
{
  return btn == (id | BUTTON_LONG_RELEASE_IND);
}

void handle_inputs(const byte btn)
{
  // Toggle active state
  if (!active)
  {
    if (is_button_long_released(btn, CHANGE_SETTING_BUTTON))
    {
      active = true;
      digitalWrite(STATUS_LED, HIGH);
    }
  }
  else
  {
    if (is_button_long_released(btn, CHANGE_SETTING_BUTTON))
    {
      queue_end = true;
    }
  }

  // Setup configuration
  if (is_button_short_released(btn, CHANGE_SETTING_BUTTON))
  {
    setting_mode = static_cast<Setting::Value>((setting_mode + 1) % Setting::N_ITEMS);
    update_segment_display();
  }

  if (is_button_short_released(btn, INCREMENT_BUTTON))
  {
    switch (setting_mode)
    {
    case Setting::GAP:
    {
      height_angle = get_height_angle(++height);
      write_delay = get_write_delay(separation_interval, height_angle);
      break;
    }
    case Setting::INTERVAL:
    {
      separation_interval += 100;
      write_delay = get_write_delay(separation_interval, height_angle);

      break;
    }
    default:
      break;
    }

    update_segment_display();
  }
  else if (is_button_short_released(btn, DECREMENT_BUTTON))
  {
    switch (setting_mode)
    {
    case Setting::GAP:
    {
      height_angle = get_height_angle(--height);
      write_delay = get_write_delay(separation_interval, height_angle);
      break;
    }
    case Setting::INTERVAL:
    {
      separation_interval -= 100;
      write_delay = get_write_delay(separation_interval, height_angle);
      break;
    }
    default:
      break;
    }

    update_segment_display();
  }
}

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

      end_if_queued();
      return;
    }

    angle++;
  }

  servo.write(angle);
}

void loop()
{
  const byte btn = MFS.getButton();
  handle_inputs(btn);

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