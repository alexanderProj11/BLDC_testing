#include "Logger.h"

void Logger::init() {
    Serial.begin(115200);  // Initialize Serial communication at a baud rate of 115200
    while (!Serial) {
        // Wait for the Serial port to connect. Needed for native USB
    }
    Serial.println("Log initialized.");
}

void Logger::log(const char* message) {
    Serial.println(message);  // Print log message to the Serial monitor
}

void Logger::logSDOResponse(uint32_t id, const uint8_t* data, uint8_t len) {
    Serial.print("SDO Response from ID: ");
    Serial.println(id, HEX);
    Serial.print("Data: ");
    for (uint8_t i = 0; i < len; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void Logger::saveLog() {
    Serial.println("Log finalized.");
}
