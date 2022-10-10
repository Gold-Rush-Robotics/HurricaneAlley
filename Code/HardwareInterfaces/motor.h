#include "utilFunc.h";

class Motor{
    int directionPin;
    int speedPin;
    bool reversed;
    Motor(int directionPin, int speedPin);
    void reverse();
    bool isReversed();
    void setPower(double power);
};