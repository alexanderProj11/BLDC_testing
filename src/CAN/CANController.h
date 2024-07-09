#ifndef CANCONTROLLER_H
#define CANCONTROLLER_H

#include <ACAN_T4.h>

class CANController {
public:
    static void begin();
    static void sendNMTCommand(uint8_t command, uint8_t nodeId);
    static void sendSDOCommand(uint8_t nodeId, uint16_t index, uint8_t subIndex, int32_t value);
    static void sendPDOCommand(uint8_t nodeId, int32_t velocity);
    static void requestHealthStatus(uint8_t nodeId);
    static void requestPosition(uint8_t nodeId);
    static void dispatchMessages();
    static int32_t getPosition(uint8_t nodeId);
    static void emergencyStop(uint8_t nodeId); // Emergency stop function
    static void setMotorState(uint8_t nodeId, uint8_t state); // Set motor state function
};

#endif
