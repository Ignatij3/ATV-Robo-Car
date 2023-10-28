#include "../engine_controller/engine_controller.h"
#include "../oled/ssd1306.h"
#include "central_controller.h"

// _controllerInitEngines initializes engines and PWM timers.
void _controllerInitEngines(void) {
    initializeEngines();
}

// enableCar enables cars engines.
// Function configures engines to run forward.
void enableCar(void) {
    setPower_OLED("ON");
    setEnginesDirection(false);
    setSpeed(0, false);
}

// disableCar halts the car, disabling it's engines and setting PWM signal duty rate to 0.
// To continue driving, call 'enableCar'.
void disableCar(void) {
    setPower_OLED("OFF");
    setSpeed(0, false);
    turnOffEngines();
}

// accelerate will increase speed of the car by 'step'.
// Once speed is at allowed maximum, accelerate does not change it.
void accelerate(uint8_t step) {
    increaseSpeed(step);
}

// decelerate will decrease speed of the car by 'step'.
// Once speed is 0, decelerate does not change it.
void decelerate(uint8_t step) {
    decreaseSpeed(step);
}

// setEngineSpeed sets engine duty cycle proportional to fraction speed/255.
void setEngineSpeed(uint8_t speed) {
    setSpeed(speed, isReverse());
}

// reverseEngines reverses engine torque direction.
void reverseEngines(void) {
    setSpeed(getSpeed(), !isReverse());
}
