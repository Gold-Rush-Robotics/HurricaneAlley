#include "servo.h"
#include <iostream>

//MIGHT NOT WORK !!! THIS IS SUS

int PWM_MAX = 4095;
int PWM_MIN = 1000;

Servo::Servo(int pin, std::shared_ptr<PCA9685> pca){
    Servo::pin = pin;
    Servo::pca9685 = pca;
}
/*!
    this sets the position for the servo in normal mode to pursue
    \param degree 0-360 for the end effector
    \return 0 unless something goes wrong
*/
int Servo::setPosition(double degree, double max){
    double power = ((double) PCA9685_VALUE_MAX - 1.0) * (degree/max);
    pca9685->Write(CHANNEL(pin), VALUE(degree));
    
}
/*!
    this sets the speed for a CR servo
    0 - Full speed CCW
    0.5 - off
    1 - Full Speed CW
*/
int Servo::setSpeed(double speed){
    double power = (PCA9685_VALUE_MAX - 1) * (speed);
    pca9685->Write(CHANNEL(pin), VALUE(power));
}