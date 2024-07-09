#ifndef CANCONTROLLER_H
#define CANCONTROLLER_H

#include <ACAN_T4.h>

class CANController {
public:
    static void begin();
    static void sendNMTCommand(uint8_t command, uint8_t nodeId);
};

#endif
