extern "C" {
#include "src/central_controller/central_controller.h"
}

void setup() {
    Serial.begin(9600);
    initializeVehicleControls(10);
}

void loop() {
    setMode(AUTOMATIC);
    run();
}
