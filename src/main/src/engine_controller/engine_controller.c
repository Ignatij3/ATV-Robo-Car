#include "engine_controller.h"

pinMode(in1, OUTPUT); 
pinMode(in2, OUTPUT); 
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);

struct car{
    uint16_t car_speed;
}

void turnLeft(uint16_t angle)
{
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
}

void turnRight(uint16_t angle)
{
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
}

void turnAround(uint16_t angle)
{

}

void setSpeed(uint8_t speed, char reverse)
{
    if (char){
        car_speed *= -1;
    }
    car_speed = speed;
}

void increaseSpeed(uint8_t speed)
{
    car_speed += speed;
}

void decreaseSpeed(uint8_t speed)
{
    car_speed -= speed;
}
