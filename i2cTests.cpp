#include "bcm2835.h"
#include "pca9685.h"
#include <iostream>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdint.h>
#include <bitset>


// compiler command
// g++ -o i2cTest i2cTests.cpp -I "../../rpidmx512/lib-pca9685/include/" -L "../../rpidmx512/lib-pca9685/lib_linux/" -l pca9685 -l bcm2835; sudo ./i2cTest


int main(){
    bcm2835_init();
    if(!bcm2835_i2c_begin()){
        std::cout << "Program not run as root" << std::endl;
    }

    uint32_t length = 10;

    char buffer[10]={0};
    char *address = &buffer[0];
    uint8_t slaveAddress = 0x29;
    bcm2835_i2c_setSlaveAddress(slaveAddress);
    bcm2835_i2c_read(address , length);
    for(int i=0 ; i<length ; i++)
    {
        std::cout << "val: " << std::bitset<8>(buffer[i]).to_ulong() << std::endl;
    }

    return 0;
}
