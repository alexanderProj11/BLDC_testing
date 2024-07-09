// CANController.cpp
#include "CANController.h"
#include "Logger.h"

int32_t CANController::receivedValue = 0; // Store the received SDO value

void CANController::begin() {
    ACAN_T4FD_Settings settings(500 * 1000); // 500 kbps
    const uint32_t errorCode = ACAN_T4FD::can1.begin(settings);
    if (errorCode == 0) {
        Logger::log("CAN1 initialized successfully");
    } else {
        Logger::log("Error initializing CAN1: 0x" + String(errorCode, HEX));
    }
}

void CANController::writeSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex, int32_t value) {
    CANFDMessage message;
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
    ACAN_T4FD::can1.tryToSend(message);
    logMessage("Sent SDO", message);
}

int32_t CANController::readSDO(uint8_t nodeId, uint16_t index, uint8_t subIndex) {
    CANFDMessage message;
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
    ACAN_T4FD::can1.tryToSend(message);
    logMessage("Requested SDO", message);
    delay(100); // Give some time for response
    return receivedValue;
}

void CANController::writePDO(uint8_t nodeId, int32_t value) {
    CANFDMessage message;
    message.id = 0x200 + nodeId; // PDO message ID
    message.len = 8;
    message.data[0] = value & 0xFF;
    message.data[1] = (value >> 8) & 0xFF;
    message.data[2] = (value >> 16) & 0xFF;
    message.data[3] = (value >> 24) & 0xFF;
    ACAN_T4FD::can1.tryToSend(message);
    logMessage("Sent PDO", message);
}

int32_t CANController::readPDO(uint8_t nodeId) {
    CANFDMessage message;
    if (ACAN_T4FD::can1.receive(message) && message.id == (0x180 + nodeId)) { // PDO response
        receivedValue = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
        logMessage("Received PDO", message);
    }
    return receivedValue;
}

void CANController::writeEMCY(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister) {
    CANFDMessage message;
    message.id = 0x080 + nodeId; // EMCY message ID
    message.len = 8;
    message.data[0] = errorCode & 0xFF;
    message.data[1] = (errorCode >> 8) & 0xFF;
    message.data[2] = errorRegister;
    ACAN_T4FD::can1.tryToSend(message);
    logMessage("Sent EMCY", message);
}

void CANController::readEMCY(uint8_t nodeId) {
    CANFDMessage message;
    if (ACAN_T4FD::can1.receive(message) && message.id == (0x080 + nodeId)) { // EMCY response
        uint16_t errorCode = message.data[0] | (message.data[1] << 8);
        uint8_t errorRegister = message.data[2];
        Logger::log("Received EMCY from Node ID: " + String(nodeId) + ", Error Code: " + String(errorCode, HEX) + ", Error Register: " + String(errorRegister, HEX));
    }
}

void CANController::writeNMT(uint8_t nodeId, uint8_t command) {
    CANFDMessage message;
    message.id = 0x000; // NMT message ID
    message.len = 2;
    message.data[0] = command;
    message.data[1] = nodeId;
    ACAN_T4FD::can1.tryToSend(message);
    Logger::log("Sent NMT command to Node ID: " + String(nodeId) + ", Command: " + String(command, HEX));
}

void CANController::writeHeartbeat(uint8_t nodeId, uint16_t producerTime) {
    writeSDO(nodeId, 0x1017, 0x00, producerTime);
    Logger::log("Set Heartbeat producer time for Node ID: " + String(nodeId) + " to " + String(producerTime));
}

void CANController::readHeartbeat(uint8_t nodeId) {
    int32_t heartbeatTime = readSDO(nodeId, 0x1017, 0x00);
    Logger::log("Heartbeat producer time for Node ID: " + String(nodeId) + " is " + String(heartbeatTime));
}

void CANController::dispatchMessages() {
    CANFDMessage message;
    while (ACAN_T4FD::can1.receive(message)) {
        if (message.id >= 0x580 && message.id <= 0x5FF) { // SDO response range
            receivedValue = message.data[4] | (message.data[5] << 8) | (message.data[6] << 16) | (message.data[7] << 24);
            logMessage("Received SDO Response", message);
        }
    }
}

void CANController::logMessage(const char* action, const CANFDMessage& message) {
    Logger::log(String(action) + " - ID: 0x" + String(message.id, HEX) + ", Length: " + String(message.len) + ", Data: " +
        String(message.data[0], HEX) + " " + String(message.data[1], HEX) + " " + String(message.data[2], HEX) + " " +
        String(message.data[3], HEX) + " " + String(message.data[4], HEX) + " " + String(message.data[5], HEX) + " " +
        String(message.data[6], HEX) + " " + String(message.data[7], HEX));
}
