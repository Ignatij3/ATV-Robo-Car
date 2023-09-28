#include "../engine_controller/engine_controller.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"
#include "distance_sensor/HCSR04.h"
#include <util/delay.h>

#define MAX_SPEED 255
#define F_CPU 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR(baud) F_CPU / 16 / baud - 1

typedef struct {
    uint8_t minimalTolerableDistance;
    drivingMode mode;
    uint8_t speed;
} carControls;

static carControls vehicle;

static bool isCollisionSoon(void);
static void evadeCollision(void);
static drivingMode readModeChange(void);
static void accelerate(uint8_t step);
static void decelerate(uint8_t step);

// initialize performs initialization of structures for the vehicle to operate.
// minimalTolerableDistance is minimal distance between car and object in front,
// to be able to safely evade collision.
void initializeModules(uint8_t minimalTolerableDistance) {
    vehicle.minimalTolerableDistance = minimalTolerableDistance;
    vehicle.mode = NONE;
    initializeEngines();
    // registerDistanceSensor();
    serialInit(MYUBRR(BAUD));
}

// isCollision returns whether vehicle is about to collide with object in front.
static bool isCollisionSoon(void) {
    uint8_t distance = measureDistanceCm();
    return distance < vehicle.minimalTolerableDistance;
}

// evadeCollision will tank turn clockwise until there is no object in front of
// the car.
static void evadeCollision(void) {
    while (vehicle.speed > 0) {
        decelerate(20);
        _delay_ms(50);
    }
    while (isCollisionSoon()) {
        turnRight(5);
    }
}

// setMode sets specified driving mode.
void setMode(drivingMode mode) {
    if (mode != NONE) {
        vehicle.mode = mode;
    }
}

// readModeChange reads input from onboard joystick, which will signal a new
// mode being selected.
static drivingMode readModeChange(void) {
    return NONE;
}

// accelerate will gradually increase speed of the car until reaching maximum.
static void accelerate(uint8_t step) {
    writeByte('a');
    writeByte('c');
    writeByte('c');
    writeByte('\n');
    if (vehicle.speed == MAX_SPEED) {
        return;
    }

    if (vehicle.speed < MAX_SPEED - step) {
        vehicle.speed += step;
    } else {
        vehicle.speed = MAX_SPEED;
    }
    setSpeed(vehicle.speed, false);
}

// decelerate will gradually decrease speed of the car until reaching 0.
static void decelerate(uint8_t step) {
    if (vehicle.speed == 0) {
        return;
    }

    if (vehicle.speed > step) {
        vehicle.speed -= step;
    } else {
        vehicle.speed = 0;
    }
    setSpeed(vehicle.speed, false);
}

// run moves the car in whatever mode is set. To change mode, onboard joystick
// should be used. If mode is set to "NONE", function exits.
void run(void) {
    while (1) {
        _delay_ms(10); // not to change state too rapidly
        drivingMode newMode = readModeChange();
        if (newMode != NONE) {
            vehicle.mode = newMode;
        }
        switch (vehicle.mode) {
        case AUTOMATIC:
            accelerate(5);
            if (isCollisionSoon()) {
                evadeCollision();
            }
            break;

        case CONTROLLED:
            /* code */
            break;

        case SLAVE:
            /* code */
            break;

        case NONE:
            // Serial.println("No mode is selected");
            return;
        }
    }
}