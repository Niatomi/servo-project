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