#include "MotorController.h"
#include "CANController.h"
#include "Logger.h"

void MotorController::init(uint8_t nodeId) {
    Logger::log("Motor initialized for Node ID: " + String(nodeId));
}

void MotorController::setVelocity(uint8_t nodeId, int32_t velocity) {
    CANController::sendPDOCommand(nodeId, velocity);
    Logger::log("Set velocity for Node ID: " + String(nodeId) + " to " + String(velocity));
}

void MotorController::setAcceleration(uint8_t nodeId, int32_t acceleration) {
    CANController::sendSDOCommand(nodeId, 0x6044, 0x00, acceleration); // Assuming 0x6044 is the index for acceleration
    Logger::log("Set acceleration for Node ID: " + String(nodeId) + " to " + String(acceleration));
}

void MotorController::monitorHealth(uint8_t nodeId) {
    CANController::requestHealthStatus(nodeId);
    Logger::log("Requested health status for Node ID: " + String(nodeId));
}

void MotorController::moveToPosition(uint8_t nodeId, int32_t currentAngle, int32_t targetPosition, int32_t maxSpeed) {
    int32_t currentPos = CANController::getPosition(nodeId);
    int32_t delta = targetPosition - currentPos;
    int32_t halfDelta = delta / 2;
    int32_t rampUpDistance = halfDelta;
    int32_t rampDownDistance = halfDelta;
    int32_t plateauDistance = 0;

    // Calculate the trapezoidal profile distances
    if (abs(delta) > 2 * maxSpeed) {
        rampUpDistance = maxSpeed;
        rampDownDistance = maxSpeed;
        plateauDistance = abs(delta) - 2 * maxSpeed;
    }

    // Accelerate to max speed
    setAcceleration(nodeId, maxSpeed);
    setVelocity(nodeId, maxSpeed);
    delay(rampUpDistance / maxSpeed * 1000);

    // Maintain max speed
    if (plateauDistance > 0) {
        setVelocity(nodeId, maxSpeed);
        delay(plateauDistance / maxSpeed * 1000);
    }

    // Decelerate to stop
    setAcceleration(nodeId, -maxSpeed);
    setVelocity(nodeId, 0);
    delay(rampDownDistance / maxSpeed * 1000);

    Logger::log("Move to position completed for Node ID: " + String(nodeId) + ", Target Position: " + String(targetPosition));
}

void MotorController::emergencyStop(uint8_t nodeId) {
    CANController::emergencyStop(nodeId);
    Logger::log("Emergency stop executed for Node ID: " + String(nodeId));
}

