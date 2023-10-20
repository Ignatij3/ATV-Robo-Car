#ifndef ATV_LINE_FOLLOW_IR_SENSOR_H
#define ATV_LINE_FOLLOW_IR_SENSOR_H
#include <stdbool.h>
#include <stdint.h>

void initializeIR();
void read_sensor(bool *IR_L, bool *IR_R);
// void calculate_pid();
// void read_array_value();
// void motor_control();

#endif