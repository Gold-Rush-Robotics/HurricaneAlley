#include "utilFunc.h"

#include "bcm2835.h"
#include "pca9685.h"

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