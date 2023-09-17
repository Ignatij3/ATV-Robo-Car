extern "C" {
#include "src/central_controller/central_controller.h"
#include "src/engine_controller/engine_controller.h"
#include "src/serial_communication/serial_communication.h"
#include "src/constants/constants.h"
}

void setup() {
  Serial.begin(9600);
  Serial.println("TESTING");
}

void loop() {
  initTEST();
  delay(400);
  digitalWrite(BUILTIN_LED, LOW);
  delay(100);
}
