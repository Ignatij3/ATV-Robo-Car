#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "engine_controller.h"
#include <avr/io.h>

struct carspeed {
    uint8_t speed;
    bool reverse;
};

// IN1/IN3	  IN2/IN4	Spinning Direction
// Low(0)	  Low(0)	Motor OFF
// High(1)	  Low(0)	Forward
// Low(0)	  High(1)	Backward
// High(1)	  High(1)	Motor OFF

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
static void setDutyCycle(void);

// initializeEngines sets up input and enable pins.
// After setting up, the function makes sure engines do not move.
void initializeEngines(void) {
    pinMode(&PORTD, LEFT_TURN_LED_PIN, OUTPUT);
    pinMode(&PORTD, RIGHT_TURN_LED_PIN, OUTPUT);

    pinMode(&PORTB, IN1, OUTPUT);
    pinMode(&PORTB, IN2, OUTPUT);
    pinMode(&PORTB, IN3, OUTPUT);
    pinMode(&PORTB, IN4, OUTPUT);

    pinMode(&PORTD, ENA, OUTPUT);
    pinMode(&PORTD, ENB, OUTPUT);

    car.speed = 0;
    car.reverse = false;
    // stops the car, to be sure it doesn't move
    turnOffEngines();
    setSpeed(0, car.reverse);
}

// forward configures engines to turn forward.
static void forward(void) {
    digitalWrite(&PORTB, IN1, HIGH);
    digitalWrite(&PORTB, IN2, LOW);
    digitalWrite(&PORTB, IN3, HIGH);
    digitalWrite(&PORTB, IN4, LOW);
}

// backwards configures engines to turn backwards.
static void backwards(void) {
    digitalWrite(&PORTB, IN1, LOW);
    digitalWrite(&PORTB, IN2, HIGH);
    digitalWrite(&PORTB, IN3, LOW);
    digitalWrite(&PORTB, IN4, HIGH);
}

// left configures left engines to turn backwards and right engines to turn forward.
static void left(void) {
    digitalWrite(&PORTB, IN1, LOW);
    digitalWrite(&PORTB, IN2, HIGH);
    digitalWrite(&PORTB, IN3, HIGH);
    digitalWrite(&PORTB, IN4, LOW);
}

// right configures left engines to turn forward and right engines to turn backwards.
static void right(void) {
    digitalWrite(&PORTB, IN1, HIGH);
    digitalWrite(&PORTB, IN2, LOW);
    digitalWrite(&PORTB, IN3, LOW);
    digitalWrite(&PORTB, IN4, HIGH);
}

// turnOnEngines configures engines to turn either forward or backwards, depending on reverse flag.
// This function must be called first for engines tor.
void turnOnEngines(bool reverse) {
    if (reverse) {
        backwards();
    } else {
        forward();
    }
}

// turnOffEngines configures engines to not turn even if power is applied.
void turnOffEngines(void) {
    digitalWrite(&PORTB, IN1, LOW);
    digitalWrite(&PORTB, IN2, LOW);
    digitalWrite(&PORTB, IN3, LOW);
    digitalWrite(&PORTB, IN4, LOW);
}

// turnLeft stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
void turnLeft(bool (*cancelFunc)(void)) {
    setSpeed(0, car.reverse);
    left();
    bool preserveReverse = car.reverse;

    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, HIGH);
    // set engine power to maximum, to turn
    setSpeed(MAX_SPEED, true);
    while (cancelFunc()) {
    }
    setSpeed(0, true);
    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, LOW);

    car.reverse = preserveReverse;
    turnOnEngines(car.reverse);
}

// turnRight stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
void turnRight(bool (*cancelFunc)(void)) {
    setSpeed(0, car.reverse);
    right();
    bool preserveReverse = car.reverse;

    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, HIGH);
    // set engine power to maximum, to turn
    setSpeed(MAX_SPEED, true);
    while (cancelFunc()) {
    }
    setSpeed(0, true);
    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, LOW);

    car.reverse = preserveReverse;
    turnOnEngines(car.reverse);
}

// setDutyCycle sets motor duty cycle equivalent to speed of the car.
static void setDutyCycle(void) {
    analogWrite(&PORTD, ENA, car.speed);
    analogWrite(&PORTD, ENB, car.speed);
}

// setSpeed sets desired speed and direction of movement.
// It changes speed instantly.
void setSpeed(uint8_t speed, bool reverse) {
    car.speed = speed;
    if (reverse != car.reverse) {
        car.reverse = reverse;
        turnOnEngines(car.reverse);
    }
    setDutyCycle();
}

// increaseSpeed increases speed by 'step'.
// If the speed is at maximum, it does nothing.
void increaseSpeed(uint8_t step) {
    if (car.speed == MAX_SPEED) {
        return;
    }

    if (car.speed < MAX_SPEED - step) {
        car.speed += step;
    } else {
        car.speed = MAX_SPEED;
    }
    setDutyCycle();
}

// decreaseSpeed decreases speed by 'step'.
// If the speed is 0, it does nothing.
void decreaseSpeed(uint8_t step) {
    if (car.speed == 0) {
        return;
    }

    if (step > car.speed) {
        car.speed = 0;
    } else {
        car.speed -= step;
    }
    setDutyCycle();
}
