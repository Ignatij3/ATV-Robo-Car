#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

enum drivingMode {
    SLAVE,
    CONTROLLED,
    AUTOMATIC
};

bool trigglePowerState();
void setDistanceFromObject(int distance);
void setMode(drivingMode mode);
void setActivationTreshold(int t);
void setControllerTravel(int minX, int maxX, int minY, int maxY);
void controllerState(int x, int y);

#endif