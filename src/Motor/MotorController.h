#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
public:
    static void init(uint8_t nodeId);
    static void setVelocity(uint8_t nodeId, int32_t velocity);
};

#endif
