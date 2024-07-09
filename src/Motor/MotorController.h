#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
public:
    static void init(uint8_t nodeId);
    static void setVelocity(uint8_t nodeId, int32_t velocity);
    static void setAcceleration(uint8_t nodeId, int32_t acceleration);
    static void monitorHealth(uint8_t nodeId);
    static void moveToPosition(uint8_t nodeId, int32_t currentAngle, int32_t targetPosition, int32_t maxSpeed);
    static void emergencyStop(uint8_t nodeId); // Emergency stop function
};

#endif

