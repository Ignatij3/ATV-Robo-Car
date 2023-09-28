#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "engine_controller.h"
#include <avr/io.h>
#include <stdint.h>

struct carspeed {
    uint8_t leftSideSpeed;
    uint8_t rightSideSpeed;
};

// right-side motor pins
#define IN1 PINB0
#define IN2 PINB1
// left-side motor pins
#define IN3 PINB2
#define IN4 PINB3
// PWM signal pins
#define ENA PIND6
#define ENB PIND5

// defining LED turning pins
#define LEFT_TURN_LED_PIN PIND2
#define RIGHT_TURN_LED_PIN PIND3

static struct carspeed car;

static void forward(void);
static void backwards(void);
static void left(void);
static void right(void);

void initializeEngines(void) {
    pinMode(&PORTD, LEFT_TURN_LED_PIN, OUTPUT);
    pinMode(&PORTD, RIGHT_TURN_LED_PIN, OUTPUT);

    pinMode(&PORTB, IN1, OUTPUT);
    pinMode(&PORTB, IN2, OUTPUT);
    pinMode(&PORTB, IN3, OUTPUT);
    pinMode(&PORTB, IN4, OUTPUT);

    pinMode(&PORTD, ENA, OUTPUT);
    pinMode(&PORTD, ENB, OUTPUT);
    car.leftSideSpeed = 0;
    car.rightSideSpeed = 0;
}

// IN1	      IN2	    Spinning Direction
// Low(0)	  Low(0)	Motor OFF
// High(1)	  Low(0)	Forward
// Low(0)	  High(1)	Backward
// High(1)	  High(1)	Motor OFF
// same principle with IN3 and IN4

static void forward(void) {
    digitalWrite(&PORTB, IN1, HIGH);
    digitalWrite(&PORTB, IN2, LOW);
    digitalWrite(&PORTB, IN3, HIGH);
    digitalWrite(&PORTB, IN4, LOW);
}

static void backwards(void) {
    digitalWrite(&PORTB, IN1, LOW);
    digitalWrite(&PORTB, IN2, HIGH);
    digitalWrite(&PORTB, IN3, LOW);
    digitalWrite(&PORTB, IN4, HIGH);
}

static void left(void) {
    digitalWrite(&PORTB, IN1, LOW);
    digitalWrite(&PORTB, IN2, HIGH);
    digitalWrite(&PORTB, IN3, HIGH);
    digitalWrite(&PORTB, IN4, LOW);
}

static void right(void) {
    digitalWrite(&PORTB, IN1, HIGH);
    digitalWrite(&PORTB, IN2, LOW);
    digitalWrite(&PORTB, IN3, LOW);
    digitalWrite(&PORTB, IN4, HIGH);
}

// function gets the angle and rotates to the left
// by the value to the left
void turnLeft(uint16_t angle) {
    left();
    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, HIGH);
    for (uint16_t i = 0; i < angle; i++) {
        // here we need to somehow know how much degrees we need to turn
    }
    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, LOW);
    forward();
}

// function gets the angle and rotates to the
// right by the value to the left
void turnRight(uint16_t angle) {
    right();
    digitalWrite(&PORTD, RIGHT_TURN_LED_PIN, HIGH);
    for (uint16_t i = 0; i < angle; i++) {
        // here we need to somehow know how much degrees we need to turn
    }
    digitalWrite(&PORTD, RIGHT_TURN_LED_PIN, LOW);
    forward();
}

void turnAround(void) {
    turnRight(180);
}

void setSpeed(uint8_t speed, bool reverse) {
    if (reverse) {
        backwards();
    } else {
        forward();
    }
    car.leftSideSpeed = speed;
    car.rightSideSpeed = speed;
    writeByte('e');
    writeByte('n');
    writeByte('g');
    writeByte('1');
    writeByte('\n');
    // analogWrite(&PORTD, ENA, car.leftSideSpeed);
    // analogWrite(&PORTD, ENB, car.rightSideSpeed);
    digitalWrite(&PORTD, ENA, HIGH);
    digitalWrite(&PORTD, ENB, HIGH);
    writeByte('e');
    writeByte('n');
    writeByte('g');
    writeByte('2');
    writeByte('\n');
}

void increaseSpeed(uint8_t speed) // a function that increases the speed of the car
{
    if (255 - speed >= car.leftSideSpeed) { // checking for the overflow for left side
        car.leftSideSpeed = 255;            // set the maximum value for left side
    } else {
        car.leftSideSpeed += speed; // set the needed value for left side
    }

    if (255 - speed >= car.rightSideSpeed) { // checking for the overflow for right side
        car.rightSideSpeed = 255;            // set the maximum value for right side
    } else {
        car.rightSideSpeed += speed; // set the needed value for right side
    }
    // analogWrite(&PORTD, ENA, car.leftSideSpeed);
    // analogWrite(&PORTD, ENB, car.rightSideSpeed);
    digitalWrite(&PORTD, ENA, HIGH);
    digitalWrite(&PORTD, ENB, HIGH);
}

void decreaseSpeed(uint8_t speed) // a function that decreases the speed of the car
{
    if (speed > car.leftSideSpeed) { // checking for the underflow for left side
        car.leftSideSpeed = 0;       // set the minimum value for left side
    } else {
        car.leftSideSpeed -= speed; // set the needed value for left side
    }

    if (speed > car.rightSideSpeed) { // checking for the underflow for right side
        car.rightSideSpeed = 0;       // set the minimum value for right side
    } else {
        car.rightSideSpeed -= speed; // set the needed value for right side
    }
    // analogWrite(&PORTD, ENA, car.leftSideSpeed);
    // analogWrite(&PORTD, ENB, car.rightSideSpeed);
    digitalWrite(&PORTD, ENA, HIGH);
    digitalWrite(&PORTD, ENB, HIGH);
}
