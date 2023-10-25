#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "engine_controller.h"
#include <avr/io.h>

typedef struct {
    uint8_t leftSpeed;
    uint8_t rightSpeed;
    bool reverse;
} carspeed;

static carspeed car;

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

static void forward(void);
static void backwards(void);
static void left(void);
static void right(void);
static void setDutyCycle(void);
static void setSpeedEngines(bool reverse);
static void enableTurnSignals(void);
static void setLeftTurnSignal(uint8_t power);
static void setRightTurnSignal(uint8_t power);

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

    car.leftSpeed = MIN_SPEED;
    car.rightSpeed = MIN_SPEED;
    car.reverse = false;
    // stops the car, to be sure it doesn't move
    turnOffEngines();
    setSpeed(MIN_SPEED, car.reverse);
}

// setEnginesDirection configures engines to turn either forward or backwards, depending on forward flag.
// This function must be called first for engines tor.
void setEnginesDirection(bool isForward) {
    if (isForward) {
        forward();
    } else {
        backwards();
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
    setSpeed(MIN_SPEED, car.reverse);
    left();
    bool preserveReverse = car.reverse;

    setLeftTurnSignal(HIGH);
    setSpeed(MAX_SPEED, true);
    while (cancelFunc()) {
        // waiting for function to signal end of turning
    }
    setSpeed(MIN_SPEED, preserveReverse);
    setLeftTurnSignal(LOW);

    setEnginesDirection(car.reverse);
}

// turnRight stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
void turnRight(bool (*cancelFunc)(void)) {
    setSpeed(MIN_SPEED, car.reverse);
    right();
    bool preserveReverse = car.reverse;

    setRightTurnSignal(HIGH);
    setSpeed(MAX_SPEED, true);
    while (cancelFunc()) {
        // waiting for function to signal end of turning
    }
    setSpeed(MIN_SPEED, preserveReverse);
    setRightTurnSignal(LOW);

    setEnginesDirection(car.reverse);
}

// setLeftSpeed sets desired speed and direction of movement for motors on the left side of the vehicle.
// It changes speed instantly.
void setLeftSpeed(uint8_t speed, bool reverse) {
    car.leftSpeed = speed;
    setSpeedEngines(reverse);
}

// setRightSpeed sets desired speed and direction of movement for motors on the right side of the vehicle.
// It changes speed instantly.
void setRightSpeed(uint8_t speed, bool reverse) {
    car.rightSpeed = speed;
    setSpeedEngines(reverse);
}

// setSpeed sets desired speed and direction of movement for both motors.
// It changes speed instantly.
void setSpeed(uint8_t speed, bool reverse) {
    car.leftSpeed = speed;
    car.rightSpeed = speed;
    setSpeedEngines(reverse);
}

// setSpeedEngines checks if engines need to be reversed and sets PWM generator duty cycle accordingly to the speed.
static void setSpeedEngines(bool reverse) {
    if (reverse != car.reverse) {
        car.reverse = reverse;
        setEnginesDirection(car.reverse);
    }
    setDutyCycle();
}

// increaseSpeed increases speed by 'step'.
// If the speed of both sides is at maximum, it does nothing.
void increaseSpeed(uint8_t step) {
    if (car.leftSpeed == MAX_SPEED && car.rightSpeed == MAX_SPEED) {
        return;
    }

    if (car.leftSpeed > MAX_SPEED - step) {
        car.leftSpeed = MAX_SPEED;
    } else {
        car.leftSpeed += step;
    }

    if (car.rightSpeed > MAX_SPEED - step) {
        car.rightSpeed = MAX_SPEED;
    } else {
        car.rightSpeed += step;
    }

    setDutyCycle();
}

// decreaseSpeed decreases speed by 'step'.
// If the speed of both sides is 0, it does nothing.
void decreaseSpeed(uint8_t step) {
    if (car.leftSpeed == MIN_SPEED && car.rightSpeed == MIN_SPEED) {
        return;
    }

    if (car.leftSpeed < step) {
        car.leftSpeed = MIN_SPEED;
    } else {
        car.leftSpeed -= step;
    }

    if (car.rightSpeed < step) {
        car.rightSpeed = MIN_SPEED;
    } else {
        car.rightSpeed -= step;
    }

    setDutyCycle();
}

// getSpeed returns engines' average speed.
uint8_t getSpeed(void) {
    return (car.leftSpeed + car.rightSpeed) / 2;
}

// getLeftSpeed returns left engine's speed.
uint8_t getLeftSpeed(void) {
    return car.leftSpeed;
}

// getRightSpeed returns right engine's speed.
uint8_t getRightSpeed(void) {
    return car.rightSpeed;
}

// isReverse returns whether engines are turning direction opposite to forward.
bool isReverse(void) {
    return car.reverse;
}

// setDutyCycle sets motor duty cycle equivalent to speed of the car.
// As a side-effect, this function will enable or disable turn signals.
static void setDutyCycle(void) {
    analogWrite(&PORTD, ENA, car.leftSpeed);
    analogWrite(&PORTD, ENB, car.rightSpeed);
    enableTurnSignals();
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

// enableTurnSignal will turn on appropriate turn signal if the car engines' speed is not matching.
// The function will turn off turn signals if the car engines' speed is matching.
static void enableTurnSignals(void) {
    if (car.leftSpeed < car.rightSpeed) {
        setLeftTurnSignal(HIGH);
        setRightTurnSignal(LOW);
    } else if (car.leftSpeed > car.rightSpeed) {
        setRightTurnSignal(HIGH);
        setLeftTurnSignal(LOW);
    } else {
        setRightTurnSignal(LOW);
        setLeftTurnSignal(LOW);
    }
}

// setLeftTurnSignal will either turn ON or OFF left turning signal.
// User should pass either HIGH or LOW to avoid possible bugs.
static void setLeftTurnSignal(uint8_t power) {
    digitalWrite(&PORTD, LEFT_TURN_LED_PIN, power);
}

// setLeftTurnSignal will either turn ON or OFF right turning signal.
// User should pass either HIGH or LOW to avoid possible bugs.
static void setRightTurnSignal(uint8_t power) {
    digitalWrite(&PORTD, RIGHT_TURN_LED_PIN, power);
}