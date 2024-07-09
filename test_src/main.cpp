#include <Arduino.h>
#include "CANController.h"
#include "Logger.h"

const uint8_t NODE_ID = 1;

void setup() {
    Logger::init();
    CANController::begin();
    
    // Start diagnostics
    Logger::log("Starting diagnostics...");
    
    // Request various CANopen objects
    // Device Profile
    CANController::requestSDO(NODE_ID, 0x1000, 0x00); // Device type
    Logger::log("Requested device type");
    delay(100);

    // SDO Configuration
    CANController::requestSDO(NODE_ID, 0x1018, 0x01); // Identity object - vendor ID
    Logger::log("Requested vendor ID");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x1018, 0x02); // Identity object - product code
    Logger::log("Requested product code");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x1018, 0x03); // Identity object - revision number
    Logger::log("Requested revision number");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x1018, 0x04); // Identity object - serial number
    Logger::log("Requested serial number");
    delay(100);

    // PDO Configuration
    CANController::requestSDO(NODE_ID, 0x1400, 0x01); // Receive PDO communication parameter
    Logger::log("Requested RPDO1 communication parameter");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x1800, 0x01); // Transmit PDO communication parameter
    Logger::log("Requested TPDO1 communication parameter");
    delay(100);

    // NMT Configuration
    CANController::requestSDO(NODE_ID, 0x1010, 0x01); // Store parameters
    Logger::log("Requested store parameters command");
    delay(100);

    // Heartbeat Configuration
    CANController::requestSDO(NODE_ID, 0x1017, 0x00); // Heartbeat producer time
    Logger::log("Requested heartbeat producer time");
    delay(100);

    // Node Guarding
    CANController::requestSDO(NODE_ID, 0x100C, 0x00); // Guard time
    Logger::log("Requested node guard time");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x100D, 0x00); // Life time factor
    Logger::log("Requested node life time factor");
    delay(100);

    // EMCY Configuration
    CANController::requestSDO(NODE_ID, 0x1028, 0x00); // Emergency consumer
    Logger::log("Requested EMCY consumer configuration");
    delay(100);

    // SYNC Configuration
    CANController::requestSDO(NODE_ID, 0x1005, 0x00); // COB-ID SYNC
    Logger::log("Requested COB-ID SYNC");
    delay(100);

    CANController::requestSDO(NODE_ID, 0x1006, 0x00); // Communication cycle period
    Logger::log("Requested communication cycle period");
    delay(100);

    // Motor Data (Object 6410h)
    CANController::requestSDO(NODE_ID, 0x6410, 0x00); // Number of entries
    Logger::log("Requested number of entries in motor data");
    delay(100);

    // For simplicity, we will just request the first few sub-indices
    // In a real implementation, you may want to loop through all available sub-indices dynamically
    for (uint8_t subIndex = 1; subIndex <= 5; subIndex++) {
        CANController::requestSDO(NODE_ID, 0x6410, subIndex);
        Logger::log("Requested motor data, sub-index " + String(subIndex));
        delay(100);
    }

    // Finalize log
    Logger::saveLog();
    Logger::log("Log saved. Diagnostics complete.");
}

void loop() {
    CANController::dispatchMessages();
    delay(10); // Small delay to avoid tight loop
}
