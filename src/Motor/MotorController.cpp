#include "MotorController.h"
#include <ACAN_T4.h>

void MotorController::init(uint8_t nodeId) {
    // Initialization code for motor with given nodeId, if needed
}

void MotorController::setVelocity(uint8_t nodeId, int32_t velocity) {
    CANMessage message;
    message.id = 0x600 + nodeId;
    message.len = 8;
    message.data[0] = 0x2B;
    message.data[1] = 0xFF;
    message.data[2] = 0x60;
    message.data[3] = 0x00;
    message.data[4] = velocity & 0xFF;
    message.data[5] = (velocity >> 8) & 0xFF;
    message.data[6] = (velocity >> 16) & 0xFF;
    message.data[7] = (velocity >> 24) & 0xFF;
    ACAN_T4::can1.tryToSend(message);
}