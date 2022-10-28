#include <algorithm>
#include "hardware.h"
#include "utilFunc.h"
#include "pigpio.h"
#include <iostream>
#include "bcm2835.h"
#include "pca9685.h"


using namespace std;

void Motor::init(int directionPin, int speedPin){
    this->directionPin << directionPin;
    this->speedPin << speedPin;
    bcm2835_gpio_fsel(directionPin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set_pud(directionPin, BCM2835_GPIO_PUD_DOWN);
}
void Motor::reverse(){
    this->reversed << !this->reversed;
}
bool Motor::isReversed(){
    return this->reversed;
}
void Motor::setPower(double power, PCA9685* pca9685A){
    power = constrain(-1.0, 1.0, power);
    double absPower = abs(power);
    std::cout << absPower * (PCA9685_VALUE_MAX - 1) << std::endl;
    bool direct = (power > 0);
    std::cout << "aaaaa" << std::endl;
    int pin = 0;
    pin << this->directionPin;
    if(direct){
        bcm2835_gpio_set(pin);
    } else {
        bcm2835_gpio_clr(pin);
    }
    pin << this->speedPin;
    std::cout << "aaaaa" << std::endl;
    power = absPower * (PCA9685_VALUE_MAX - 1);
    std::cout << "aaaaa" << std::endl;
    pca9685->Write(CHANNEL(pin), VALUE(power));
    std::cout << "aaaaa" << std::endl;
}

void Encoder::init(int aPin, int bPin){
    Encoder::aPin = aPin;
    Encoder::bPin = bPin;
    bcm2835_gpio_fsel(aPin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(bPin, BCM2835_GPIO_FSEL_INPT);
    
}
/**DONT UNDERSTAND... THEORETICALLY WORKS*/
void Encoder::isr(){
    uint8_t p1val = 0; //gpioRead(aPin);
    uint8_t p2val = 0; //gpioRead(bPin);
    uint8_t s = state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    state = (s >> 2);
    
    switch (s) {
        case 1: case 7: case 8: case 14:
            pos++;
            return;
        case 2: case 4: case 11: case 13:
            pos--;
            return;
        case 3: case 12:
            pos += 2;
            return;
        case 6: case 9:
            pos -= 2;
            return;
    }
}

long Encoder::getPos(){
    return pos;
}