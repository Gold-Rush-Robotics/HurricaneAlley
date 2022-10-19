#include <algorithm>
#include "hardware.h"
#include "utilFunc.h"
#include "pigpio.h"

using namespace std;

Motor::Motor(int directionPin, int speedPin, PCA9685 pca9685){
    Motor::directionPin = directionPin;
    Motor::speedPin = speedPin;
    Motor::pca9685 = pca9685;
    bcm2835_gpio_fsel(directionPin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set_pud(directionPin, BCM2835_GPIO_PUD_DOWN);
}
Motor::Motor(){
    
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

Encoder::Encoder(int aPin, int bPin){
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