#ifndef CANCONTROLLER_H
#define CANCONTROLLER_H

#include <ACAN_T4.h>

class CANController {
public:
    static void begin();
    static void sendSDOCommand(uint8_t nodeId, uint16_t index, uint8_t subIndex, int32_t value);
    static void requestSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex);
    static void dispatchMessages();
};

#endif
