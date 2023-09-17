extern "C" {
#include "central_controller/central_controller.h"
#include "engine_controller/engine_controller.h"
#include "serial_communication/serial_communication.h"
}

#define BUILTIN_LED 20

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
