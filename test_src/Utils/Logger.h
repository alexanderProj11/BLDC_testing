#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SD.h>

class Logger {
public:
    static void init();
    static void log(const char* message);
    static void logSDOResponse(uint32_t id, const uint8_t* data, uint8_t len);
    static void saveLog();
private:
    static File logFile;
};

#endif
