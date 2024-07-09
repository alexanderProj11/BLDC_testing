#include "CANController.h"
#include "Logger.h"

static int32_t receivedValue = 0; // Store the received SDO value

void CANController::begin() {
    ACAN_T4_Settings settings(500 * 1000); // 500 kbps
    const uint32_t errorCode = ACAN_T4::can1.begin(settings);
    if (0 == errorCode) {
        Logger::log("CAN1 initialized successfully");
    } else {
        Logger::log("Error initializing CAN1: 0x" + String(errorCode, HEX));
    }
}

void CANController::sendSDOCommand(uint8_t nodeId, uint16_t index, uint8_t subIndex, int32_t value) {
    CANMessage message;
    message.id = 0x600 + nodeId; // SDO message ID
    message.len = 8;
    message.data[0] = 0x23; // SDO command specifier for expedited transfer
    message.data[1] = index & 0xFF; // Low byte of the index
    message.data[2] = (index >> 8) & 0xFF; // High byte of the index
    message.data[3] = subIndex; // Sub-index
    message.data[4] = value & 0xFF; // Low byte of the value
    message.data[5] = (value >> 8) & 0xFF;
    message.data[6] = (value >> 16) & 0xFF;
    message.data[7] = (value >> 24) & 0xFF; // High byte of the value
    ACAN_T4::can1.tryToSend(message);
    Logger::log("Sent SDO Command to Node ID: " + String(nodeId) + ", Index: 0x" + String(index, HEX) + ", SubIndex: 0x" + String(subIndex, HEX) + ", Value: " + String(value));
}

void CANController::requestSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex) {
    CANMessage message;
    message.id = 0x600 + nodeId; // SDO message ID
    message.len = 8;
    message.data[0] = 0x40; // SDO command specifier for upload request
    message.data[1] = index & 0xFF; // Low byte of the index
    message.data[2] = (index >> 8) & 0xFF; // High byte of the index
    message.data[3] = subIndex; // Sub-index
    message.data[4] = 0x00; // Unused
    message.data[5] = 0x00; // Unused
    message.data[6] = 0x00; // Unused
    message.data[7] = 0x00; // Unused
    ACAN_T4::can1.tryToSend(message);
    Logger::log("Requested SDO from Node ID: " + String(nodeId) + ", Index: 0x" + String(index, HEX) + ", SubIndex: 0x" + String(subIndex, HEX));
}

void CANController::dispatchMessages() {
    CANMessage message;
    if (ACAN_T4::can1.receive(message)) {
        if (message.id == (0x580 + NODE_ID)) { // SDO response
            receivedValue = message.data[4] | (message.data[5] << 8) | (message.data[6] << 16) | (message.data[7] << 24);
            Logger::logSDOResponse(message.id, message.data, message.len);
        }
    }
}

