#include "engine_controller.h"

struct carspeed {
    uint8_t leftsideSpeed; //creating speed variable for left side of car 
    uint8_t rightsideSpeed; //creating speed variable for left side of car 
};

#define IN1 2
#define IN2 7  //defining engine pins
#define IN3 10
#define IN4 15

#define ENA 1
#define ENB 9  

#define LEFT_TURN_SIGNAL 40  //THERE ARE INCORRECT
#define RIGHT_TURN_SIGNAL 400  //defining turning pins
static struct carspeed car;

void initialisaion(void){
    pinmode(LEFT_TURN_SIGNAL, OUTPUT); //turn declaration
    pinmode(RIGHT_TURN_SIGNAL, OUTPUT);

    pinMode(IN1, OUTPUT);  //declaration 4pins from H-Bridge to output
    pinMode(IN2, OUTPUT);  
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    car.leftsideSpeed=0;
    car.rightsideSpeed=0;
}

// IN1	      IN2	    Spinning Direction
// Low(0)	  Low(0)	Motor OFF
// High(1)	  Low(0)	Forward
// Low(0)	  High(1)	Backward
// High(1)	  High(1)	Motor OFF
//same principle with IN3 and IN4


void forward(void) //switch pins to go forward
{
    digitalWrite(IN1, 1); 
    digitalWrite(IN2, 0);   //the right side go forward
    digitalWrite(IN3, 1);   //the left side go forward
    digitalWrite(IN4, 0);
}

void backwards(void) //switch pins to go backwards
{
    digitalWrite(IN1, 0); 
    digitalWrite(IN2, 1);   //the right side go backward
    digitalWrite(IN3, 0);   //the left side go backward
    digitalWrite(IN4, 1);
}

void left(void) //switch pins to turn left
{
    digitalWrite(IN1, 0); 
    digitalWrite(IN2, 1);   //the right side go backwards
    digitalWrite(IN3, 1);   //the left side go forward
    digitalWrite(IN4, 0);
}

void right(void) //switch pins to turn right
{
    digitalWrite(IN1, 1); 
    digitalWrite(IN2, 0);   //the right side go forward
    digitalWrite(IN3, 0);   //the left side go backwards
    digitalWrite(IN4, 1);
}

void turnLeft(uint16_t angle) //function gets the angle and rotates to the left by the value to the left
{
    digitalWrite(LEFT_TURN_SIGNAL, 1);  //turning on the left turn signal
    left();
    for (uint16_t i=0; i<angle; i++){
        //here we need to somehow know how much degrees we need to turn
    }
    digitalWrite(LEFT_TURN_SIGNAL, 0); //turning off the left turn signal
    forward(); //the car continue going forward
}

void turnRight(uint16_t angle)  //function gets the angle and rotates to the right by the value to the left
{
    digitalWrite(RIGHT_TURN_SIGNAL, 1);  //turning on the right turn signal
    right();
    for (uint16_t i=0; i<angle; i++){
        //here we need to somehow know how much degrees we need to turn
    }
    digitalWrite(RIGHT_TURN_SIGNAL, 0); //turning off the right turn signal
    forward(); //the car continue going forward
}

void turnAround(void) //function turn the car aroung(180 degrees)
{
    turnRight(180);
    //maybe we need to add there some if from sensor
}

void setSpeed(uint8_t speed, bool reverse) //sets the right speed and reverse means the opposite side
{
    if(reverse){
        backwards();
    }
    car.leftsideSpeed = speed;  
    car.rightsideSpeed = speed;  //setting the desired speed for the car
    analogWrite(ENA, car.leftsideSpeed);
    analogWrite(ENB, car.rightsideSpeed);  //sending the speed to our engines
}

void increaseSpeed(uint8_t speed)  //a function that increases the speed of the car
{
    if (255 - speed >= car.leftsideSpeed){  //checking for the overflow for left side
        car.leftsideSpeed = 255;     //set the maximum value for left side
    } else{
        car.leftsideSpeed += speed;   //set the needed value for left side
    }
        
    if (255 - speed >= car.rightsideSpeed){  //checking for the overflow for right side
        car.rightsideSpeed = 255;  //set the maximum value for right side
    } else{
        car.rightsideSpeed += speed;  //set the needed value for right side
    }
    analogWrite(ENA, car.leftsideSpeed);
    analogWrite(ENB, car.rightsideSpeed);  //sending the speed to our engines
}

void decreaseSpeed(uint8_t speed)  //a function that decreases the speed of the car
{ 
    if (speed > car.leftsideSpeed){  //checking for the underflow for left side
        car.leftsideSpeed = 0;     //set the minimum value for left side
    } else{
        car.leftsideSpeed -= speed;   //set the needed value for left side
    }
        
    if (speed > car.rightsideSpeed){  //checking for the underflow for right side
        car.rightsideSpeed = 0;  //set the minimum value for right side
    } else{
        car.rightsideSpeed -= speed;  //set the needed value for right side
    }
    analogWrite(ENA, car.leftsideSpeed);
    analogWrite(ENB, car.rightsideSpeed);  //sending the speed to our engines
}

