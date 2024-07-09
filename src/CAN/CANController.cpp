#include "CANController.h"

void CANController::begin() {
    ACAN_T4_Settings settings(500 * 1000); // 500 kbps
    const uint32_t errorCode = ACAN_T4::can1.begin(settings);
    if (0 == errorCode) {
        Serial.println("CAN1 initialized successfully");
    } else {
        Serial.print("Error initializing CAN1: 0x");
        Serial.println(errorCode, HEX);
    }
}

void CANController::sendNMTCommand(uint8_t command, uint8_t nodeId) {
    CANMessage message;
    message.id = 0x000; // NMT message ID
    message.len = 2;
    message.data[0] = command;
    message.data[1] = nodeId;
    ACAN_T4::can1.tryToSend(message);
}