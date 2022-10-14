#include <cstdio>
#include <cstdint>
#include <unistd.h>

#include "bcm2835.h"
#include "pca9685.h"

// g++ -o wpt wiringPiTest.cpp -I "../../rpidmx512/lib-pca9685/include/" -L "../../rpidmx512/lib-pca9685/lib_linux/" -l pca9685 -l bcm2835; sudo ./wpt

static int motorPins[4][3] = {{4, 0, -1}, {17, 1, 1}, {22, 3, -1}, {27, 2, 1}};
static int motorCount = 4;

int main(){
    if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		return -2;
	}

	PCA9685 pca9685;

    bcm2835_gpio_fsel(4, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set_pud(4, BCM2835_GPIO_PUD_DOWN);


    while(true){
        bcm2835_gpio_set(4);
        usleep(1'000'000);
        bcm2835_gpio_clr(4);
        usleep(1'000'000);

        /*
        for(int i = 0; i < motorCount; i++) {
            if(motorPins[i][2] < 0){
                bcm2835_gpio_clr(motorPins[i][0]);
            } else {
                bcm2835_gpio_set(motorPins[i][0]);
            }
        }
        usleep(1'000'000);
        for(int i = 0; i < motorCount; i++) {
            if(motorPins[i][2] < 0){
                bcm2835_gpio_set(motorPins[i][0]);
            } else {
                bcm2835_gpio_clr(motorPins[i][0]);
            }
        }
        usleep(1'000'000);
        */
    }
}