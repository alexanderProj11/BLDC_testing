#include <Arduino.h>
#include "CANController.h"
#include "MotorController.h"
#include "Logger.h"

const uint8_t NODE_ID_1 = 1;
const uint8_t NODE_ID_2 = 2;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    Logger::init();
    Logger::log("Initializing CAN and Motors");

    CANController::begin();
    MotorController::init(NODE_ID_1);
    MotorController::init(NODE_ID_2);
}

void loop() {
    if (Serial.available() > 0) {
        char key = Serial.read();
        switch (key) {
            case '1':
                MotorController::setVelocity(NODE_ID_1, 1000);
                Logger::log("Motor 1 velocity set to 1000");
                break;
            case '2':
                MotorController::setVelocity(NODE_ID_2, 1000);
                Logger::log("Motor 2 velocity set to 1000");
                break;
            case 'n':
                CANController::sendNMTCommand(0x01, NODE_ID_1);
                CANController::sendNMTCommand(0x01, NODE_ID_2);
                Logger::log("NMT Start command sent to all nodes");
                break;
            default:
                Logger::log("Unknown command");
                break;
        }
    }
}
