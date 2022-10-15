#include "utilFunc.h";

class Motor{
    int directionPin;
    int speedPin;
    bool reversed;
    PCA9685 pca9685;
    Motor(int directionPin, int speedPin, PCA9685 pca9685);
    void reverse();
    bool isReversed();
    void setPower(double power);
};