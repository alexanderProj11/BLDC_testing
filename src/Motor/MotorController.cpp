#include "MotorController.h"
#include "CANController.h"
#include "Logger.h"

// CANopen object dictionary indexes and sub-indexes
#define CONTROLWORD_INDEX 0x6040
#define STATUSWORD_INDEX 0x6041
#define MODES_OF_OPERATION_INDEX 0x6060
#define MODES_OF_OPERATION_DISPLAY_INDEX 0x6061

// Profile Position Mode objects
#define TARGET_POSITION_INDEX 0x607A
#define PROFILE_VELOCITY_INDEX 0x6081
#define PROFILE_ACCELERATION_INDEX 0x6083
#define PROFILE_DECELERATION_INDEX 0x6084

// CANopen controlword bits
#define SHUTDOWN_COMMAND 0x06
#define SWITCH_ON_COMMAND 0x07
#define ENABLE_OPERATION_COMMAND 0x0F
#define QUICK_STOP_COMMAND 0x02
#define FAULT_RESET_COMMAND 0x80

// Statusword bits
#define STATUS_OPERATION_ENABLE 0x0027
#define STATUS_SWITCHED_ON 0x0021
#define STATUS_READY_TO_SWITCH_ON 0x0020
#define STATUS_FAULT 0x0008
#define STATUS_FAULT_REACTION_ACTIVE 0x004F

void MotorController::init(uint8_t nodeId) {
    Logger::log("Initializing motor for Node ID: " + String(nodeId));
    setControlword(nodeId, SHUTDOWN_COMMAND);
    if (!waitForStatus(nodeId, 0x006F, STATUS_READY_TO_SWITCH_ON, 1000)) {
        Logger::log("Failed to transition to READY_TO_SWITCH_ON for Node ID: " + String(nodeId));
        return;
    }
    setControlword(nodeId, SWITCH_ON_COMMAND);
    if (!waitForStatus(nodeId, 0x006F, STATUS_SWITCHED_ON, 1000)) {
        Logger::log("Failed to transition to SWITCHED_ON for Node ID: " + String(nodeId));
        return;
    }
    setControlword(nodeId, ENABLE_OPERATION_COMMAND);
    if (!waitForStatus(nodeId, 0x006F, STATUS_OPERATION_ENABLE, 1000)) {
        Logger::log("Failed to transition to OPERATION_ENABLE for Node ID: " + String(nodeId));
    }
}

void MotorController::setVelocity(uint8_t nodeId, int32_t velocity) {
    CANController::writeSDO(nodeId, PROFILE_VELOCITY_INDEX, 0x00, velocity);
    Logger::log("Set velocity for Node ID: " + String(nodeId) + " to " + String(velocity));
}

void MotorController::setAcceleration(uint8_t nodeId, int32_t acceleration) {
    CANController::writeSDO(nodeId, PROFILE_ACCELERATION_INDEX, 0x00, acceleration);
    Logger::log("Set acceleration for Node ID: " + String(nodeId) + " to " + String(acceleration));
}

void MotorController::setDeceleration(uint8_t nodeId, int32_t deceleration) {
    CANController::writeSDO(nodeId, PROFILE_DECELERATION_INDEX, 0x00, deceleration);
    Logger::log("Set deceleration for Node ID: " + String(nodeId) + " to " + String(deceleration));
}

void MotorController::setTargetPosition(uint8_t nodeId, int32_t position) {
    CANController::writeSDO(nodeId, TARGET_POSITION_INDEX, 0x00, position);
    Logger::log("Set target position for Node ID: " + String(nodeId) + " to " + String(position));
}

void MotorController::moveToPosition(uint8_t nodeId, int32_t currentAngle, int32_t targetPosition, int32_t maxSpeed) {
    setVelocity(nodeId, maxSpeed);
    setAcceleration(nodeId, maxSpeed / 2);
    setDeceleration(nodeId, maxSpeed / 2);
    setTargetPosition(nodeId, targetPosition);
    setControlword(nodeId, 0x3F); // Assuming 0x3F starts motion
    Logger::log("Started move to position for Node ID: " + String(nodeId) + ", Target Position: " + String(targetPosition));
}

void MotorController::monitorHealth(uint8_t nodeId) {
    uint16_t statusword = getStatusword(nodeId);
    Logger::log("Health status for Node ID: " + String(nodeId) + " is 0x" + String(statusword, HEX));
}

void MotorController::emergencyStop(uint8_t nodeId) {
    setControlword(nodeId, QUICK_STOP_COMMAND);
    Logger::log("Emergency stop executed for Node ID: " + String(nodeId));
}

void MotorController::setControlword(uint8_t nodeId, uint16_t controlword) {
    CANController::writeSDO(nodeId, CONTROLWORD_INDEX, 0x00, controlword);
    Logger::log("Set controlword for Node ID: " + String(nodeId) + " to 0x" + String(controlword, HEX));
}

uint16_t MotorController::getStatusword(uint8_t nodeId) {
    return CANController::readSDO(nodeId, STATUSWORD_INDEX, 0x00);
}

void MotorController::setOperationMode(uint8_t nodeId, int8_t mode) {
    CANController::writeSDO(nodeId, MODES_OF_OPERATION_INDEX, 0x00, mode);
    Logger::log("Set operation mode for Node ID: " + String(nodeId) + " to " + String(mode));
}

int8_t MotorController::getOperationMode(uint8_t nodeId) {
    return (int8_t)CANController::readSDO(nodeId, MODES_OF_OPERATION_DISPLAY_INDEX, 0x00);
}

void MotorController::faultReset(uint8_t nodeId) {
    setControlword(nodeId, FAULT_RESET_COMMAND);
    if (!waitForStatus(nodeId, 0x004F, 0x0000, 1000)) { // Wait for fault reset
        Logger::log("Failed to reset fault for Node ID: " + String(nodeId));
    }
    Logger::log("Fault reset for Node ID: " + String(nodeId));
}

bool MotorController::waitForStatus(uint8_t nodeId, uint16_t statusMask, uint16_t expectedStatus, uint32_t timeout) {
    uint32_t startTime = millis();
    while (millis() - startTime < timeout) {
        uint16_t statusword = getStatusword(nodeId);
        if ((statusword & statusMask) == expectedStatus) {
            return true;
        }
        delay(10); // Small delay to prevent bus overload
    }
    return false;
}
