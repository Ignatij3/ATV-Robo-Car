extern "C" {
#include "src/central_controller/central_controller.h"
}

void setup() { initializeVehicleControls(10); }

void loop() {
    setMode(AUTOMATIC);
    run();
}
