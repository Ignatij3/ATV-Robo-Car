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

// -----------TEMPORARY-----------
#define IN3 PIND3
#define IN4 PIND4
// -----------TEMPORARY-----------

// PWM signal pins
#define ENA PIND6
#define ENB PIND5

#define DEBUG
#ifdef DEBUG
#include "../oled/ssd1306.h"
#include <stdio.h>
#include <stdlib.h>
#define condwr                                                                                                         \
    {                                                                                                                  \
        char *s = malloc(sizeof(char) * 5);                                                                            \
        sprintf(s, "l%ur%u", digitalRead(&PORTC, IN1), digitalRead(&PORTC, IN2));                                      \
        SSD1306_SetPosition(0, 7);                                                                                     \
        SSD1306_DrawString(s);                                                                                         \
        free(s);                                                                                                       \
        SSD1306_UpdateScreen(OLED_ADDRESS);                                                                            \
    }
#else
#define condwr
#endif

static void forward(void);
static void backwards(void);
static void left(void);
static void right(void);
static void setDutyCycle(void);
static void setSpeedEngines(bool reverse);

#include "../serial_communication/serial_communication.h"
#include <util/delay.h>
void TESTING(void) {
    setSpeed(255, false);
    pinMode(&PORTB, PINB5, OUTPUT);

start:
    digitalWrite(&PORTB, PINB5, HIGH);
    forward();
    writeStringF("forward: 1010\r\n");
    _delay_ms(4000);

    digitalWrite(&PORTB, PINB5, LOW);
    left();
    writeStringF("left: 0110\r\n");
    _delay_ms(4000);

    backwards();
    writeStringF("backwards: 0101\r\n");
    _delay_ms(4000);

    right();
    writeStringF("right: 1001\r\n");
    _delay_ms(4000);

    goto start;
}

// initializeEngines sets up input and enable pins.
// After setting up, the function makes sure engines do not move.
void initializeEngines(void) {
    pinMode(&PORTC, IN1, OUTPUT);
    pinMode(&PORTC, IN2, OUTPUT);

    // -----------TEMPORARY-----------
    pinMode(&PORTD, IN4, OUTPUT);
    pinMode(&PORTD, IN3, OUTPUT);
    // -----------TEMPORARY-----------

    pinMode(&PORTD, ENA, OUTPUT);
    pinMode(&PORTD, ENB, OUTPUT);

    car.leftSpeed = MIN_SPEED;
    car.rightSpeed = MIN_SPEED;
    car.reverse = false;
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
// As a side-effect, this function will enable or disable turn signals.
static void setDutyCycle(void) {
    analogWrite(&PORTD, ENA, car.leftSpeed);
    analogWrite(&PORTD, ENB, car.rightSpeed);
}

// forward configures engines to turn forward.
static void forward(void) {
    condwr;
    // -----------TEMPORARY-----------
    /*
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, HIGH);
    */
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, LOW);
    digitalWrite(&PORTD, IN3, HIGH);
    digitalWrite(&PORTD, IN4, LOW);
    // -----------TEMPORARY-----------
}

// backwards configures engines to turn backwards.
static void backwards(void) {
    condwr;
    // -----------TEMPORARY-----------
    /*
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, LOW);
    */
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, HIGH);
    digitalWrite(&PORTD, IN3, LOW);
    digitalWrite(&PORTD, IN4, HIGH);
    // -----------TEMPORARY-----------
}

// left configures left engines to turn backwards and right engines to turn forward.
static void left(void) {
    condwr;
    // -----------TEMPORARY-----------
    /*
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, LOW);
    */
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, HIGH);
    digitalWrite(&PORTD, IN3, HIGH);
    digitalWrite(&PORTD, IN4, LOW);
    // -----------TEMPORARY-----------
}

// right configures left engines to turn forward and right engines to turn backwards.
static void right(void) {
    condwr;
    // -----------TEMPORARY-----------
    /*
    digitalWrite(&PORTC, IN1, LOW);
    digitalWrite(&PORTC, IN2, HIGH);
    */
    digitalWrite(&PORTC, IN1, HIGH);
    digitalWrite(&PORTC, IN2, LOW);
    digitalWrite(&PORTD, IN3, LOW);
    digitalWrite(&PORTD, IN4, HIGH);
    // -----------TEMPORARY-----------
}
