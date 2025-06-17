#include <Arduino.h>
#include <Servo.h>
#include "utils.h"

#define SERVO_PIN 0

#define GEAR_PITCH 30 // millimeters

Servo servo;

bool active = false;
int heightAngle = 0;
int separationInterval = 0;
int writeDelay = 0;

void setup()
{
  Serial.begin(9600);

  // Attach servo
  servo.attach(SERVO_PIN);

  // Reset servo to contact state
  servo.write(0);
}

void loop()
{
  if (Serial.available())
  {
    const String string = Serial.readString();

    // Start oscillation
    if (!active && string.equalsIgnoreCase("start"))
    {
      active = true;
      Serial.println("Started oscillation");
    }

    // Stop oscillation
    else if (active && string.equalsIgnoreCase("stop"))
    {
      active = false;
      Serial.println("Stopped oscillation");
    }

    // Set parameters (should be done first)
    else if (string.startsWith("set:"))
    {
      String *parameters = new String[2];
      splitAtFirst(string.substring(4), ':', parameters);

      const int height = parameters[0].toInt();
      heightAngle = 180 * height / (PI * GEAR_PITCH);

      separationInterval = parameters[1].toInt();
      writeDelay = (separationInterval / 2) / heightAngle;
    }
    return;
  }

  // Contact and separation functionality
  if (active)
  {
    // Separation
    for (int angle = 0; angle <= heightAngle; angle++)
    {
      servo.write(angle);
      delay(writeDelay);
    }

    // Contact
    for (int angle = heightAngle; angle >= 0; angle--)
    {
      servo.write(angle);
      delay(writeDelay);
    }
  }
}