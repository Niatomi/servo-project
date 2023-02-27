#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

const int SERVO_LENGTH = 3;
int servoPins[SERVO_LENGTH] = {8, 9};
Servo servo[SERVO_LENGTH];

bool lox_exists = true;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup()
{
  Serial.begin(9600);
  if (!lox.begin())
  {
    Serial.println("Distance sensor is disabled");
    while (1)
    {
      delay(1000);
    }
  }

  for (int i = 0; i < SERVO_LENGTH; i++)
  {
    servo[i].attach(servoPins[i]);
    servo[i].write(0);
  }
}

void smooth_move(Servo servo, int angle, int spd)
{
  int curr_angle = servo.read();
  int diff = curr_angle - angle;
  for (int i = 0; i < abs(diff); i++)
  {
    if (curr_angle < angle)
      servo.write(curr_angle + i);
    else
      servo.write(curr_angle - i);
    delay((10 - spd) * 10);
  }
}

String divide_string(String str_in, int part)
{
  String out, str = str_in;
  byte div_ind = str.indexOf(' ');
  for (int i = 0; i < part; i++)
  {
    out = str.substring(0, div_ind);
    str = str.substring(div_ind + 1);
    div_ind = str.indexOf(' ');
  }
  return out;
}

void wait()
{
  while (Serial.available() == 0)
  {
  }
}

void move_by_angles(int angle_fi, int angle_o, int speed)
{
  smooth_move(servo[0], angle_fi, speed);
  delay(100);
  smooth_move(servo[1], angle_o, speed);
  delay(100);

  Serial.println(lox.readRange());
}

void parse_and_excecute_commands(String cmd)
{
  cmd.trim();
  int angle_fi = divide_string(cmd, 1).toInt();
  int angle_o = divide_string(cmd, 2).toInt();
  int servo_speed = divide_string(cmd, 3).toInt();

  move_by_angles(angle_fi, angle_o, servo_speed);
}

void read_serial()
{
  Serial.println("Enter start and commands:");
  wait();
  String cmd = Serial.readString();

  if (cmd != "start\n")
  {
    Serial.println("Enter \"start\" instead...");
    return;
  }

  String cmds = "";
  Serial.println("Cmd mode init...");
  wait();

  cmd = Serial.readString();
  parse_and_excecute_commands(cmd);
}

void loop()
{
  read_serial();
}