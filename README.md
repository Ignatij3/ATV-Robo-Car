# ATV Robo Car
In this repository lies the code which controls robotic car that me and my team had made. The car should be drivable in three modes:
1. slave - following a black line on the ground;
2. controlled - being controlled by a remote controller (laptop running a terminal application in our case);
3. autonomous - driving around freely and avoiding obstacles;

### Development environment
Project was developed in VSCode, then compiled by AVR Toolchain (`avr-gcc`, `avrdude`).
To compile the project, Makefile was called, which you can find in 'src' folder.

### Module structure
Since at first we used Arduino IDE to manage the project, we had decided to write our own module which will mimic arduino library functions, providing the same functionality - `ino_libs`.
Virtually all other modules reference `ino_libs` to get access to vital functions, such as `pinMode()` and others.
Majority of modules are made as a "driver" to a specific sensor or hardware interface in the ATMega328P microcontroller, all of them can be found in `src/libs`.

### Control logic
It was decided that all control will be given to "user" of the libs - `src/main.c`.
In that file you will find logic to control the car as the customer requested and also proper startup/shutdown sequence.
**Note:** Ideally, the microcontroller should never shut down.
