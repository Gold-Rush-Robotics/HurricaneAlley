#include "I2CPeripheral.h"
#include <wiringPiI2C.h>

namespace PiPCA9685{
    I2CPeripheral::I2CPeripheral(const uint8_t address){
        ConnectToPeripheral(address);
    }

    void I2CPeripheral::WriteRegisterByte(const uint8_t register_address, const uint8_t value) {
        wiringPiI2CWriteReg8(bus_fd, register_address, value);
    }

    uint8_t I2CPeripheral::ReadRegisterByte(const uint8_t register_address){
        return wiringPiI2CReadReg8(bus_fd, register_address);
    }

    void I2CPeripheral::ConnectToPeripheral(const uint8_t address) {
        bus_fd = wiringPiI2CSetup(address);
    }
}

