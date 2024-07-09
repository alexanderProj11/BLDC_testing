#include "Logger.h"
#include <SD.h>

File logFile;

void Logger::init() {
    Serial.begin(9600);
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("Initialization failed!");
        return;
    }
    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.println("Log initialized.");
        logFile.close();
    } else {
        Serial.println("Error opening log file");
    }
}

void Logger::log(const char* message) {
    Serial.println(message);
    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.println(message);
        logFile.close();
    }
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

    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.print("SDO Response from ID: ");
        logFile.println(id, HEX);
        logFile.print("Data: ");
        for (uint8_t i = 0; i < len; i++) {
            logFile.print(data[i], HEX);
            logFile.print(" ");
        }
        logFile.println();
        logFile.close();
    }
}

void Logger::saveLog() {
    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.println("Log finalized.");
        logFile.close();
    }
}
