#include <Arduino.h>
#include "MotorController.h"
#include "CANController.h"
#include "Logger.h"

const uint8_t NODE_ID = 1;

void setup() {
    Logger::init();
    CANController::begin();
    
    MotorController::init(NODE_ID);

    // Transition to Ready to Switch On
    if (MotorController::setControlword(NODE_ID, SHUTDOWN_COMMAND)) { // Shutdown command
        delay(100);

        if (MotorController::setControlword(NODE_ID, SWITCH_ON_COMMAND)) { // Switch On command
            delay(100);

            if (MotorController::setControlword(NODE_ID, ENABLE_OPERATION_COMMAND)) { // Enable Operation command
                delay(100);

                // Set operation mode to Profile Position Mode
                if (MotorController::setOperationMode(NODE_ID, 1)) { // 1 = Profile Position Mode
                    delay(100);

                    // Example move to position
                    int32_t currentAngle = 0; // Initial position
                    int32_t targetPosition = 1000; // Target position
                    int32_t maxSpeed = 500; // Maximum speed
                    MotorController::moveToPosition(NODE_ID, currentAngle, targetPosition, maxSpeed);
                } else {
                    Logger::log("Failed to set operation mode.");
                }
            } else {
                Logger::log("Failed to enable operation.");
            }
        } else {
            Logger::log("Failed to switch on.");
        }
    } else {
        Logger::log("Failed to shutdown.");
    }
}

void loop() {
    // Continuously monitor health and status
    MotorController::monitorHealth(NODE_ID);
    delay(100); // Check health every 100 ms

    // Check for emergency stop command
    if (Serial.available() > 0) {
        char key = Serial.read();
        if (key == 'e') {
            MotorController::emergencyStop(NODE_ID);
        } else if (key == 'r') {
            MotorController::faultReset(NODE_ID);
            // Re-enable operation after fault reset
            if (MotorController::setControlword(NODE_ID, SHUTDOWN_COMMAND)) { // Shutdown command
                delay(100);
                if (MotorController::setControlword(NODE_ID, SWITCH_ON_COMMAND)) { // Switch On command
                    delay(100);
                    MotorController::setControlword(NODE_ID, ENABLE_OPERATION_COMMAND); // Enable Operation command
                }
            }
        }
    }

    CANController::dispatchMessages();
}
