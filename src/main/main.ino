extern "C" {
#include "src/central_controller/central_controller.h"
}

void setup() {
    //  Serial.begin(9600);
    Serial.println("11");
    initializeModules(10);
    Serial.println("12");
}

void loop() {
    Serial.println("21");
    setMode(AUTOMATIC);
    Serial.println("22");
    run();
    Serial.println("23");
}
