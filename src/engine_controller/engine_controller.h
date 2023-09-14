#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H

void turnLeft(short degrees);
void turnRight(short degrees);
void turnAround(short degrees);
void setSpeed(char speedUnits, char reverse);
void increaseSpeed(char speedUnits);
void decreaseSpeed(char speedUnits);

#endif