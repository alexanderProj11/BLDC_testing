#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
public:
    static void init(uint8_t nodeId);
    static void setVelocity(uint8_t nodeId, int32_t velocity);
    static void setAcceleration(uint8_t nodeId, int32_t acceleration);
    static void setDeceleration(uint8_t nodeId, int32_t deceleration);
    static void setTargetPosition(uint8_t nodeId, int32_t position);
    static void moveToPosition(uint8_t nodeId, int32_t currentAngle, int32_t targetPosition, int32_t maxSpeed);
    static void monitorHealth(uint8_t nodeId);
    static void emergencyStop(uint8_t nodeId);
    static void setControlword(uint8_t nodeId, uint16_t controlword);
    static uint16_t getStatusword(uint8_t nodeId);
    static void setOperationMode(uint8_t nodeId, int8_t mode);
    static int8_t getOperationMode(uint8_t nodeId);
    static void faultReset(uint8_t nodeId);
};

#endif
