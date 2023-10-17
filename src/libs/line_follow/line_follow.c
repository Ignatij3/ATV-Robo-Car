#include "line_follow.h"
#include "../engine_controller/engine_controller.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include <avr/io.h>
#define turnSpeed 20
#define leftIR PINC1
#define rightIR PINC2
#define centerIR PINC3

// IR sensor declaration
bool IR_L;
bool IR_C;
bool IR_R;
/*// PID stuff for future use
int error = 0;
int P = 0;
int I = 0;
int D = 0;
int PID_value = 0;
int previous_I = 0;
int previous_error = 0;
int initial_motor_speed = 100;

// PID GAINS - Adjust as needed
float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;

void calculate_pid()//calculating pid
{
    P = error;
    I = I + previous_I;
    D = error-previous_error;

    PID_value = (Kp*P) + (Ki*I) + (Kd*D);

    previous_I=I;
    previous_error=error;
}

void read_array_value() {
  ir_array[0]=digitalRead(A0);
  ir_array[1]=digitalRead(A1);
  ir_array[2]=digitalRead(A2);

  /* Sensor array value
   0    0   0 = NO LINE
   0    0   1
   0    1   1
   0    1   0
   1    1   0
   1    0   0
   1    1   1 = ALL LINE


  if((ir_array[0]==0)&&(ir_array[1]==0)&&(ir_array[2]==0)) {
    // NO LINE FOUND - MOVE RANDOMLY
  }

  else if((ir_array[0]==0)&&(ir_array[1]==0)&&(ir_array[2]==1)) {
    error=2; // Way too much to the right
  }

  else if((ir_array[0]==0)&&(ir_array[1]==1)&&(ir_array[2]==1)) {
    error=1; // Too much to the right
  }

  else if((ir_array[0]==0)&&(ir_array[1]==1)&&(ir_array[2]==0)) {
    error=0; // Car is perfectly centered
  }

  else if((ir_array[0]=1)&&(ir_array[1]==1)&&(ir_array[2]==0)) {
    error=-1; // Too much to the left
  }

  else if((ir_array[0]==1)&&(ir_array[1]==0)&&(ir_array[2]==0)) {
    error=-2; // Way too much to the left
  }

  else if((ir_array[0]==1)&&(ir_array[1]==1)&&(ir_array[2]==1)) {
  // ALL SENSORS DETECT BLACK LINE - WHAT TO DO?
  }
}

void motor_control()//motor control
{
    // Calculating the effective motor speed:
    int left_motor_speed = initial_motor_speed-PID_value;
    int right_motor_speed = initial_motor_speed+PID_value;

    // The motor speed should not exceed the max PWM value
    constrain(left_motor_speed,0,255);
    constrain(right_motor_speed,0,255);

    analogWrite(9,initial_motor_speed-PID_value);   //Left Motor Speed
    analogWrite(10,initial_motor_speed+PID_value);  //Right Motor Speed

    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
}
 */

// Sensors define
void initializeIR() {
    pinMode(&PORTC, leftIR, INPUT);
    pinMode(&PORTC, rightIR, INPUT);
    pinMode(&PORTC, centerIR, INPUT);
}

void follow_line() {
    IR_L = digitalRead(&PORTC, leftIR);
    IR_R = digitalRead(&PORTC, rightIR);
    // ir_array[2]=digitalRead(A2);

    /*Sensor array value
     0    0   0 = NO LINE
     0    0   1
     0    1   1
     0    1   0
     1    1   0
     1    0   0
     1    1   1 = ALL LINE
    */

    if ((IR_L == 0) && (IR_C == 0) && (IR_R == 0)) {
        serialWrite("Automatic");
    }

    else if ((IR_L == 1) && (IR_C == 0) && (IR_R == 0)) {
        serialWrite("turnRight()");
        serialWrite("setSpeed(turnSpeed,0)");
    }

    else if ((IR_L == 0) && (IR_C == 1) && (IR_R == 0)) {
        serialWrite("Forward()");
        serialWrite("setSpeed(100,0)");
    }

    else if ((IR_L == 0) && (IR_C == 0) && (IR_R == 1)) {
        serialWrite("turnLeft()");
        serialWrite("setSpeed(turnSpeed,0)");
    }

    else if ((IR_L == 1) && (IR_C == 1) && (IR_R == 1)) {
        serialWrite("Automatic");
    }
}
