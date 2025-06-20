#include <Arduino.h>
#include <Servo.h>
#include "utils.h"

#define SERVO_PIN 8

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
  servo.write(180);
}

void loop()
{
  if (Serial.available())
  {
    const String string = Serial.readString();
    Serial.println(string);
    // Start oscillation
    if (!active)
    {
      if (string.equalsIgnoreCase("start"))
      {
        active = true;
        Serial.println("Started oscillation");
      }
      else if (string.equalsIgnoreCase("separate"))
      {
        servo.write(100);
      }
      else if (string.equalsIgnoreCase("contact"))
      {
        servo.write(180);
      }
      else if (string.startsWith("set:"))
      {
        String *parameters = new String[2];
        splitAtFirst(string.substring(4), ':', parameters);

        const int height = parameters[0].toInt();
        // const int height = 10;
        heightAngle = 180 * height / (PI * GEAR_PITCH);

        separationInterval = parameters[1].toInt();
        // separationInterval = 1000;
        writeDelay = (separationInterval / 2) / heightAngle;

        Serial.println("Height:");
        Serial.println(height);
        Serial.println("Interval:");
        Serial.println(separationInterval);

        Serial.println("writeDelay:");
        Serial.println(writeDelay);
        }
    }

    // Stop oscillation
    else if (active && string.equalsIgnoreCase("stop"))
    {
      active = false;
      servo.write(180);
      Serial.println("Stopped oscillation");
    }
    return;
  }

  // Contact and separation functionality
  if (active)
  {
    // Separation
    for (int angle = 180; angle >= 180 - heightAngle; angle--)
    {
      servo.write(angle);
      delay(writeDelay);
    }

    // Contact
    for (int angle = 180 - heightAngle; angle <= 180; angle++)
    {
      servo.write(angle);
      delay(writeDelay);
    }
  }
}