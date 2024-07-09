#include <Arduino.h>
#include "CANController.h"
#include "Logger.h"

const uint8_t NODE_ID = 1;

void setup() {
    Logger::init();
    CANController::begin();
    
    // Start diagnostics
    Logger::log("Starting diagnostics...");
    
    // Request device ID
    CANController::requestSDO(NODE_ID, 0x1018, 0x01);
    Logger::log("Requested device ID");

    delay(100); // Give some time for response

    // Request device type
    CANController::requestSDO(NODE_ID, 0x1000, 0x00);
    Logger::log("Requested device type");

    delay(100); // Give some time for response

    // Request sensor data
    CANController::requestSDO(NODE_ID, 0x6000, 0x00);
    Logger::log("Requested sensor data");

    delay(100); // Give some time for response

    // Finalize log
    Logger::saveLog();
    Logger::log("Log saved. Diagnostics complete.");
}

void loop() {
    CANController::dispatchMessages();
    delay(10); // Small delay to avoid tight loop
}

