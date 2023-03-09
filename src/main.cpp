#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

#define LID_IS_CONNECTED

const int SERVO_LENGTH = 3;
int servoPins[SERVO_LENGTH] = {8, 9};
Servo servo[SERVO_LENGTH];

bool lox_exists = true;

#ifdef LID_IS_CONNECTED
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#endif
void setup()
{
#ifdef LID_IS_CONNECTED
  Serial.begin(115200);
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

String read_serial()
{
  wait();
  String cmd = Serial.readString();
  return cmd;
}

void py_function(int min_fi, int max_fi,
                 int min_tetta, int max_tetta,
                 int cols, int rows,
                 int stop_time, int speed)
{

  // fi - угол поврорта
  // tetta - угол наколна
  int x_step = (max_fi - min_fi) / rows;
  int y_step = (max_fi - min_fi) / cols;
  unsigned long measure_start_time = millis();
  String data_line = "";
  for (int y = 0; y < rows; y++)
  {
    if (millis() - measure_start_time > stop_time)
    {
      return;
    }

    smooth_move(servo[1], min_tetta + y * y_step, speed);
    data_line = "";
    for (int x = 0; x < cols; x++)
    {
#ifdef LID_IS_CONNECTED
      uint16_t data = lox.readRange();
      data_line += data;
#endif
      data_line += " ";
      smooth_move(servo[0], min_fi + x * x_step, speed);
    }
    Serial.println(data_line);
    data_line = "";
  }
}

void parse_and_excecute_commands(String cmd)
{
  // cmd.trim();
  // String begin_cmd = divide_string(cmd, 0);
  int min_fi = divide_string(cmd, 1).toInt();
  int max_fi = divide_string(cmd, 2).toInt();

  int min_tetta = divide_string(cmd, 3).toInt();
  int max_tetta = divide_string(cmd, 4).toInt();

  int cols = divide_string(cmd, 5).toInt();
  int rows = divide_string(cmd, 6).toInt();

  int stop_time = divide_string(cmd, 7).toInt();
  int speed = divide_string(cmd, 8).toInt();

  py_function(min_fi, max_fi,
              min_tetta, max_tetta,
              cols, rows,
              stop_time, speed);
}

void loop()
{
  String command = read_serial();
  parse_and_excecute_commands(command);
}