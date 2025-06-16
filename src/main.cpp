#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN 1
#define GEAR_TEETH 12
#define GEAR_PITCH 0.03 // meters

const float DELTA_ANGLE = 360.0 / GEAR_TEETH;
const float DELTA_HEIGHT = 2 * PI * (GEAR_PITCH / 2) / GEAR_TEETH;

Servo servo;
bool active = false;
int heightAngle;

void setContactState();
void setSeparationState();

void setup()
{
  Serial.begin(9600); // For debugging purposes

  // Attach servo
  servo.attach(SERVO_PIN);

  // Reset servo to contact state
  setContactState();
}

void loop()
{
  if (!active && Serial.available())
  {
    String string = Serial.readString();

    if (string.equalsIgnoreCase("start"))
    {
      active = true;
    }
    else if (string.startsWith("set:"))
    {
      int height = string.substring(4).toInt();
    }

    return;
  }
}

void setContactState()
{
  servo.write(0);
}
