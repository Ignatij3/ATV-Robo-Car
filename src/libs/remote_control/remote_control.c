#include "../serial_communication/serial_communication.h"
#include "remote_control.h"

typedef struct {
    uint8_t a;
    uint8_t b;
} moveVector;

static volatile moveVector vec;

// pairWithModule will send 'init' to bluetooth module, expecting answer.
// The function returns whether received answer matches expected answer.
// Expected answer is 'conndone'.
bool pairWithModule(void) {
    writeString("init");
    char answer[9];
    readNBytes(&answer, 8);
    return answer == "conndone";
}

// readMovementVector will wait data, containing information about vector of movement.
// This function will block until everything is received.
void readMovementVector(void) {
    vec.a = readByte();
    vec.b = readByte();
}

// vectorToEngineSpeed will translate last updated vector to left and right engines' speed.
void vectorToEngineSpeed(uint8_t *left, uint8_t *right) {
    // second byte's first 7 least significant bits are length of the vector divided by 2
    uint8_t vLen = vec.b << 1;
    // first byte contains 8 most significant bits
    // and most significant bit of second byte is least significant bit of angle
    uint16_t vAngle = (vec.a << 1) | (vec.b >> 7);
}