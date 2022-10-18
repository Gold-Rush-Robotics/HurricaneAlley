#include <stdint.h>
#include "utilFunc.h"
#include "bcm2835.h"
#include "pca9685.h"

class Encoder{
    public:
    int aPin;
    int bPin;
    volatile long pos = 0;
    volatile uint8_t state;
    Encoder(int aPin, int bPin);
    Encoder();
    void isr();
    long getPos();
};

class Motor{
    public:
    int directionPin;
    int speedPin;
    bool reversed;
    PCA9685 pca9685;
    Motor(int directionPin, int speedPin, PCA9685 pca9685);
    Motor();
    void reverse();
    bool isReversed();
    void setPower(double power);
};