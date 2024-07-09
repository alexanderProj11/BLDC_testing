#include <Arduino.h>
#include "CANController.h"
#include "MotorController.h"
#include "Logger.h"

const uint8_t NODE_ID = 1;

void monitorHealth();

void setup() {
    Logger::init();
    CANController::begin();
    MotorController::init(NODE_ID);

    // Set up a timer to monitor health status every 0.1 seconds
    Timer1.initialize(100000); // 0.1 second in microseconds
    Timer1.attachInterrupt(monitorHealth);
}

void loop() {
    if (Serial.available() > 0) {
        char key = Serial.read();
        switch (key) {
            case 'v':
                MotorController::setVelocity(NODE_ID, 1000);
                Logger::log("Motor velocity set to 1000");
                break;
            case 's':
                MotorController::setVelocity(NODE_ID, -1000);
                Logger::log("Motor velocity set to -1000");
                break;
            case 'a':
                MotorController::setAcceleration(NODE_ID, 500);
                Logger::log("Motor acceleration set to 500");
                break;
            case 'n':
                CANController::sendNMTCommand(0x01, NODE_ID);
                Logger::log("NMT Start command sent to node");
                break;
            case 'p':
                CANController::sendNMTCommand(0x80, NODE_ID); // Pre-operational state
                Logger::log("Node set to pre-operational state");
                break;
            case 'o':
                CANController::sendNMTCommand(0x01, NODE_ID); // Operational state
                Logger::log("Node set to operational state");
                break;
            case 'm':
                int32_t targetPosition = 1000; // Example target position
                int32_t maxSpeed = 500; // Example max speed
                MotorController::moveToPosition(NODE_ID, CANController::getPosition(NODE_ID), targetPosition, maxSpeed);
                Logger::log("Motor moved to position");
                break;
            case 'h':
                MotorController::monitorHealth(NODE_ID);
                Logger::log("Motor health status requested");
                break;
            case 'e':
                MotorController::emergencyStop(NODE_ID);
                Logger::log("Emergency stop triggered manually");
                break;
            default:
                Logger::log("Unknown command");
                break;
        }
    }
    CANController::dispatchMessages();
}

// Health monitoring function
void monitorHealth() {
    MotorController::monitorHealth(NODE_ID);
}
