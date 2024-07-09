#ifndef CANCONTROLLER_H
#define CANCONTROLLER_H

#include <ACAN_T4.h>

class CANController {
public:
    static void begin();
    static void writeSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex, int32_t value);
    static int32_t readSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex);
    static void writePDO(uint8_t nodeId, int32_t value);
    static int32_t readPDO(uint8_t nodeId);
    static void writeEMCY(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister);
    static void readEMCY(uint8_t nodeId);
    static void writeNMT(uint8_t nodeId, uint8_t command);
    static void writeHeartbeat(uint8_t nodeId, uint16_t producerTime);
    static void readHeartbeat(uint8_t nodeId);
    static void dispatchMessages();
private:
    static void logMessage(const char* action, const CANFDMessage& message);
    static int32_t receivedValue;
};

#endif
