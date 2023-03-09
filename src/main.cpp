#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <math.h>
#include "servo_config.h"
#include "statistics_count.h"

Servo servo[SERVO_LENGTH];
#include "utils.h"

#ifdef LID_IS_CONNECTED
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#endif

void setup()
{
  Serial.begin(115200);
#ifdef LID_IS_CONNECTED
  if (!lox.begin())
  {
    Serial.println("Distance sensor is disabled");
    while (1)
    {
      delay(1000);
    }
  }
#endif

  for (int i = 0; i < SERVO_LENGTH; i++)
  {
    servo[i].attach(servoPins[i]);
    smooth_move(servo[i], 90, 8);
  }
}

void move_manipulator(float x, float y, float z)
{
  float alfa_base = get_alfa_base(x, y);
  float x_one = get_x_one(x, l_one, alfa_base);
  float y_one = get_y_one(y, l_one, alfa_base);

  float alfa_three = get_alfa_three(x, y, z, x_one, y_one, l_two, l_three);
  float alfa_two = get_alfa_two(x, y, z, l_two, l_three);
  float alfa_one = get_alfa_one(alfa_two, alfa_three);

  Serial.println(alfa_three);
  Serial.println(alfa_two);
  Serial.println(alfa_one);

  // smooth_move(servo[2], alfa_three, servo_speed);
  // smooth_move(servo[1], alfa_two, servo_speed);
  // smooth_move(servo[0], alfa_one, servo_speed);
}

// Config vars

void parse_and_excecute_commands(String cmd)
{
  String begin_cmd = divide_string(cmd, 1);
  if (begin_cmd.equals("speed"))
  {
    servo_speed = divide_string(cmd, 2).toInt();
  }
  else if (begin_cmd.equals("move"))
  {
    float x = divide_string(cmd, 2).toFloat();
    float y = divide_string(cmd, 3).toFloat();
    float z = divide_string(cmd, 4).toFloat();
    // move chichaaaaaaaaaaaaa
    move_manipulator(x, y, z);
  }
  else if (begin_cmd.equals("grab_open"))
  {
    // opens grab
  }
  else if (begin_cmd.equals("close_grab"))
  {
    // closes grab
  }
  else if (begin_cmd.equals("grab_rotate_angle"))
  {
    // float angle = divide_string(cmd, 2).toFloat();
    // moves angle
  }
}

void loop()
{
  String command = read_serial();
  parse_and_excecute_commands(command);
}
