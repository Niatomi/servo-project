float get_alfa_base(float x, float y)
{
    float alfa_base = (180 / M_PI) * atan(y / x);
    return alfa_base;
}

float get_x_one(float x, float l_one, float alfa_base)
{
    float x_one = x - (l_one * cos(alfa_base));
    return x_one;
}

float get_y_one(float y, float l_one, float alfa_base)
{
    float y_one = y - (l_one * sin(alfa_base));
    return y_one;
}

float get_alfa_two(float x, float y, float z, float l_two, float l_three)
{

    float alfa_two = (180 - (180 / M_PI)) * (acos((pow(l_three, 2) + pow(l_two, 2) - pow(x, 2) - pow(y, 2) - pow(z, 2)) /
                                                  (2 * l_three * l_two)));
    return alfa_two;
}

float get_alfa_three(float x, float y, float z, float x_one, float y_one, float l_two, float l_three)
{
    float alfa_three = (90 - (180 / M_PI)) * ((acos((pow(x_one, 2) + pow(y_one, 2) + pow(z, 2) + pow(l_two, 2) - pow(l_three, 2)) /
                                                    (2 * l_three * sqrt(pow(x, 2) + pow(y, 2))))) -
                                              atan(z / sqrt(pow(x, 2) + pow(y, 2))));
    return alfa_three;
}

float get_alfa_one(float alfa_two, float alfa_three)
{
    float alfa_one = 90 - alfa_two - alfa_three;
    return alfa_one;
}