#include <cstdio>
#include <cstdint>
#include <unistd.h>

#include "bcm2835.h"
#include "pca9685.h"

// compiler command: 
// g++ -o libTest libTest.cpp -I "../../rpidmx512/lib-pca9685/include/" -L "../../rpidmx512/lib-pca9685/lib_linux/" -l pca9685 -l bcm2835

int main(int argc, char **argv) {
	if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		return -2;
	}

	uint16_t OnValue, OffValue;
	PCA9685 pca9685;

	pca9685.Dump();

	pca9685.SetFrequency(1000);

	//pca9685.SetFullOn(CHANNEL(8), true);

    int channelNum = 3;

    /*
    0: BR 
    1: BL
    2: FL
    3: FR
    */

    for(int i = 0; i < 4; i++) {
        pca9685.Write(CHANNEL(i), VALUE(PCA9685_VALUE_MAX/2));
        sleep(2);
        pca9685.SetFullOff(CHANNEL(i), true); 
    }

    for(int i = 0; i < 16; i++) {
        pca9685.SetFullOff(CHANNEL(i), true); 
    }

    pca9685.Dump();

    return 0;
}