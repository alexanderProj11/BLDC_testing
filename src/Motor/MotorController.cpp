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

void MotorController::init(uint8_t nodeId) {
    Logger::log("Motor initialized for Node ID: " + String(nodeId));
    // Set initial controlword to Shutdown command
    setControlword(nodeId, SHUTDOWN_COMMAND);
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

    // Set controlword to start movement
    setControlword(nodeId, 0x3F); // Assuming the controlword to start motion
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
    Logger::log("Fault reset for Node ID: " + String(nodeId));
}
