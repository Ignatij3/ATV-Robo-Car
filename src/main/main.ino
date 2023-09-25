extern "C" {
#include "src/central_controller/central_controller.h"
}

void setup() {
  initializeModules(10);
}

void loop() {
    setMode(AUTOMATIC);
    run();
}
