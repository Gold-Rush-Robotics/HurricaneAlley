#include <algorithm>
#include "motor.h"
#include "../Utilities/utilFunc.h"
#include <iostream>
#include "bcm2835.h"
#include "pca9685.h"

Motor::Motor(int directionPin, int speedPin, std::shared_ptr<PCA9685> pca){
    Motor::directionPin = directionPin;
    Motor::speedPin = speedPin;
    pca9685 = pca;
    bcm2835_gpio_fsel(directionPin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set_pud(directionPin, BCM2835_GPIO_PUD_DOWN);
}

void Motor::reverse(){
    reversed = !reversed;
}

bool Motor::isReversed(){
    return reversed;
}

void Motor::setPower(double power){
    std::cout << "power IN: " << power << std::endl;
    power = constrain(-1.0, 1.0, power);
    double absPower = abs(power);

    if(reversed && power < 0){
        bcm2835_gpio_clr(directionPin);
        std::cout << "INV: " << reversed << std::endl;
    } else if(reversed && power > 0) {
        bcm2835_gpio_set(directionPin);
    } else if(!reversed && power < 0){
        bcm2835_gpio_set(directionPin);
    } else {
        bcm2835_gpio_clr(directionPin);
        std::cout << "FWD: " << reversed << std::endl;
    }

    power = absPower * (PCA9685_VALUE_MAX - 1);
    std::cout << "Power OUT: " << power << std::endl;
    pca9685->Write(CHANNEL(speedPin), VALUE(power));
}