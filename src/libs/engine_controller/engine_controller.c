#include "../central_controller/central_controller.h"
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

//  IN1	  IN2	driving direction
//    0	    0	backwards
//    0	    1	right
//    1	    0	left
//    1	    1	forward

// motor pins
#define IN1 PINC3
#define IN2 PINC2
// PWM signal pins
#define ENA PIND6
#define ENB PIND5

static void forward(void);
static void backwards(void);
static void left(void);
static void right(void);
static void setDutyCycle(void);
static void setSpeedEngines(bool reverse);

// initializeEngines sets up input and enable pins.
// After setting up, the function makes sure engines do not move.
void initializeEngines(void) {
    pinMode(&PORTC, IN1, OUTPUT);
    pinMode(&PORTC, IN2, OUTPUT);

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
void setEnginesDirection(bool reverse) {
    if (!reverse) {
        forward();
    } else {
        backwards();
    }
}

// turnOffEngines configures engines to not turn even if power is applied.
void turnOffEngines(void) {
    analogWrite(&PORTD, ENA, MIN_SPEED);
    analogWrite(&PORTD, ENB, MIN_SPEED);
}

// tankTurnLeft stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
void tankTurnLeft(bool (*cancelFunc)(void)) {
    decreaseSpeed(MAX_SPEED);
    left();

    increaseSpeed(MAX_SPEED);
    // waiting for function to signal end of turning
    while (cancelFunc()) {
        updateCarMetrics();
    }
    decreaseSpeed(MAX_SPEED);

    setEnginesDirection(car.reverse);
}

// tankTurnRight stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
void tankTurnRight(bool (*cancelFunc)(void)) {
    decreaseSpeed(MAX_SPEED);
    right();

    increaseSpeed(MAX_SPEED);
    // waiting for function to signal end of turning
    while (cancelFunc()) {
        updateCarMetrics();
    }
    decreaseSpeed(MAX_SPEED);

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
}

// forward configures engines to turn forward.
static void forward(void) {
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, HIGH);
}

// backwards configures engines to turn backwards.
static void backwards(void) {
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, LOW);
}

// left configures left engines to turn backwards and right engines to turn forward.
static void left(void) {
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, LOW);
}

// right configures left engines to turn forward and right engines to turn backwards.
static void right(void) {
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, HIGH);
}
