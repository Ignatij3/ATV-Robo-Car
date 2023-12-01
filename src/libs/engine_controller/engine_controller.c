#include "engine_controller.h"
#include "../central_controller/central_controller.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include <avr/io.h>

typedef struct {
    uint8_t leftSpeed;
    uint8_t rightSpeed;
    bool reverse;
} carspeed;

static carspeed car;

//  ENA   ENB
//   5V    5V
//
//  IN1	  IN2	IN3	  IN4	driving direction
//  REV	    1	REV	    1	backwards
//  REV	    0	FOR	    0	right
//  FOR	    0	REV	    1	left
//  PWM	    0	PWM	    0	forward
//
//  PWM - engine speed is determined by duty cycle of PWM signal.
//  REV - engine speed is determined by difference of maximum duty cycle value and
//        current duty cycle (dc) of PWM signal (255-dc).

// motor pins
// IN1, IN3 are connected to wave generators
#define IN1 PIND6
#define IN2 PINC2
#define IN3 PIND5
#define IN4 PINC3

static void forward(void);
static void backwards(void);
static void left(void);
static void right(void);
static void setDutyCycle(void);
static void setSpeedEngines(bool reverse);

typedef enum {
    D_FORWARD,
    D_LEFT,
    D_RIGHT,
    D_BACKWARDS,
} drDirection;

static drDirection dir;

// initializeEngines sets up input and enable pins.
// After setting up, the function makes sure engines do not move.
void initializeEngines(void) {
    pinMode(&PORTD, IN1, OUTPUT);
    pinMode(&PORTC, IN2, OUTPUT);
    pinMode(&PORTD, IN3, OUTPUT);
    pinMode(&PORTC, IN4, OUTPUT);

    car.leftSpeed = MIN_SPEED;
    car.rightSpeed = MIN_SPEED;

    car.reverse = false;
    dir = D_FORWARD;
    // stops the car, to be sure it doesn't move
    turnOffEngines();
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

// turnOffEngines configures engines to not turn.
void turnOffEngines(void) {
    setSpeed(0, car.reverse);
}

// enableTurning will stop the car and set engines working in opposite to each other direction to tank turn.
// direction argument is responsible for setting direction of the turning.
// If direction > 0, the car turns right, otherwise left.
void enableTurning(int8_t direction) {
    if (direction >= 0) {
        right();
    } else {
        left();
    }
}

// tankTurn stops the car to perform tank turn.
// The function will constantly call cancelFunc until it returns false.
// Once cancelFunc returns false, the function restores engine torque vector.
// On function exit, car remains stationary.
// direction argument is responsible for setting direction of the turning.
// If direction > 0, the car turns right, otherwise left.
void tankTurn(bool (*cancelFunc)(void), int8_t direction) {
    decreaseSpeed(MAX_SPEED);
    enableTurning(direction);

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
static void setDutyCycle(void) {
    // for explanation of function's duty cycle consult with comment at
    // the beginning of the file
    switch (dir) {
    case D_FORWARD:
        analogWrite(&PORTD, IN1, car.leftSpeed);
        analogWrite(&PORTD, IN3, car.rightSpeed);
        break;

    case D_BACKWARDS:
        analogWrite(&PORTD, IN1, MAX_SPEED - car.leftSpeed);
        analogWrite(&PORTD, IN3, MAX_SPEED - car.rightSpeed);
        break;

    case D_LEFT:
        analogWrite(&PORTD, IN1, car.leftSpeed);
        analogWrite(&PORTD, IN3, MAX_SPEED - car.rightSpeed);
        break;

    case D_RIGHT:
        analogWrite(&PORTD, IN1, MAX_SPEED - car.leftSpeed);
        analogWrite(&PORTD, IN3, car.rightSpeed);
        break;

    default:
        analogWrite(&PORTD, IN1, MIN_SPEED);
        analogWrite(&PORTD, IN3, MIN_SPEED);
        break;
    }
}

// forward configures engines to turn forward.
static void forward(void) {
    digitalWrite(&PORTC, IN2, LOW);
    digitalWrite(&PORTC, IN4, LOW);
    dir = D_FORWARD;
}

// backwards configures engines to turn backwards.
static void backwards(void) {
    digitalWrite(&PORTC, IN2, HIGH);
    digitalWrite(&PORTC, IN4, HIGH);
    dir = D_BACKWARDS;
}

// left configures left engines to turn backwards and right engines to turn forward.
static void left(void) {
    digitalWrite(&PORTC, IN2, LOW);
    digitalWrite(&PORTC, IN4, HIGH);
    dir = D_LEFT;
}

// right configures left engines to turn forward and right engines to turn backwards.
static void right(void) {
    digitalWrite(&PORTC, IN2, HIGH);
    digitalWrite(&PORTC, IN4, LOW);
    dir = D_RIGHT;
}
