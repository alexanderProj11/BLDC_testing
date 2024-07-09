#include "CANController.h"
#include "Logger.h"

static int32_t currentPosition = 0; // Store the current position

void CANController::begin() {
    ACAN_T4_Settings settings(500 * 1000); // 500 kbps
    const uint32_t errorCode = ACAN_T4::can1.begin(settings);
    if (0 == errorCode) {
        Logger::log("CAN1 initialized successfully");
    } else {
        Logger::log("Error initializing CAN1: 0x" + String(errorCode, HEX));
    }
}

void CANController::sendNMTCommand(uint8_t command, uint8_t nodeId) {
    CANMessage message;
    message.id = 0x000; // NMT message ID
    message.len = 2;
    message.data[0] = command;
    message.data[1] = nodeId;
    ACAN_T4::can1.tryToSend(message);
    Logger::log("Sent NMT Command: 0x" + String(command, HEX) + " to Node ID: " + String(nodeId));
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

void CANController::sendPDOCommand(uint8_t nodeId, int32_t velocity) {
    CANMessage message;
    message.id = 0x200 + nodeId; // PDO message ID
    message.len = 8;
    message.data[0] = velocity & 0xFF;
    message.data[1] = (velocity >> 8) & 0xFF;
    message.data[2] = (velocity >> 16) & 0xFF;
    message.data[3] = (velocity >> 24) & 0xFF;
    ACAN_T4::can1.tryToSend(message);
    Logger::log("Sent PDO Command to Node ID: " + String(nodeId) + ", Velocity: " + String(velocity));
}

void CANController::requestHealthStatus(uint8_t nodeId) {
    // Example: requesting a status word (index 0x6041, subindex 0x00)
    sendSDOCommand(nodeId, 0x6041, 0x00, 0);
    Logger::log("Requested Health Status for Node ID: " + String(nodeId));
}

void CANController::requestPosition(uint8_t nodeId) {
    // Example: requesting the current position (index 0x6064, subindex 0x00)
    sendSDOCommand(nodeId, 0x6064, 0x00, 0);
    Logger::log("Requested Position for Node ID: " + String(nodeId));
}

void CANController::dispatchMessages() {
    CANMessage message;
    if (ACAN_T4::can1.receive(message)) {
        if (message.id == (0x580 + nodeId)) { // SDO response
            // Check if the message contains position data
            if (message.data[1] == 0x64 && message.data[2] == 0x60) {
                currentPosition = message.data[4] | (message.data[5] << 8) | (message.data[6] << 16) | (message.data[7] << 24);
                Logger::log("Received Position Data: " + String(currentPosition));
            }
            Logger::logSDOResponse(message.id, message.data, message.len);
        } else if (message.id == (0x80 + nodeId)) { // EMCY message
            emergencyStop(nodeId);
            Logger::log("Emergency stop triggered by EMCY message for Node ID: " + String(nodeId));
        }
    }
}

int32_t CANController::getPosition(uint8_t nodeId) {
    return currentPosition;
}

void CANController::emergencyStop(uint8_t nodeId) {
    sendPDOCommand(nodeId, 0); // Decelerate to zero velocity
    setMotorState(nodeId, 0x02); // Change state to stopped
    Logger::log("Emergency stop executed for Node ID: " + String(nodeId));
}

void CANController::setMotorState(uint8_t nodeId, uint8_t state) {
    sendNMTCommand(state, nodeId);
    Logger::log("Set Motor State for Node ID: " + String(nodeId) + " to State: 0x" + String(state, HEX));
}
