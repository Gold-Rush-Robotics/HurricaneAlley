#include <algorithm>
#include "motor.h"
#include "../Utilities/utilFunc.h"
#include <iostream>
#include "bcm2835.h"
#include "pca9685.h"

void Motor::Motor(int directionPin, int speedPin, std::shared_ptr<PCA9685> pca){
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
    power = constrain(-1.0, 1.0, power);
    double absPower = abs(power);
    std::cout << absPower * (PCA9685_VALUE_MAX - 1) << std::endl;
    if(power * int(reversed) > 0){
        bcm2835_gpio_set(directionPin);
    } else {
        bcm2835_gpio_clr(directionPin);
    }
    power = absPower * (PCA9685_VALUE_MAX - 1);
    pca9685->Write(CHANNEL(speedPin), VALUE(power));
}