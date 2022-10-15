
#include <algorithm>;


#include "motor.h";
#include "utilFunc.h";


#include "bcm2835.h"
#include "pca9685.h"

using namespace std;

Motor::Motor(int directionPin, int speedPin, PCA9685 pca9685){
    Motor::directionPin = directionPin;
    Motor::speedPin = speedPin;
    Motor::pca9685 = pca9685;
    bcm2835_gpio_fsel(4, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set_pud(4, BCM2835_GPIO_PUD_DOWN);
}
void Motor::reverse(){
    reversed = !reversed;
}
bool Motor::isReversed(){
    return reversed;
}
void Motor::setPower(double power){
    power = constrain(-1, 1, power);
    int absPower = int(abs(power));
    int direct = int(reversed) * power > 0 ? 0 : 1;
    direct? bcm2835_gpio_set(directionPin) : bcm2835_gpio_clr(directionPin);
    pca9685.Write(CHANNEL(speedPin), VALUE(absPower * (PCA9685_VALUE_MAX - 1)));
}