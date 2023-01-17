#include "servo.h"

//MIGHT NOT WORK !!! THIS IS SUS

Servo::Servo(int pin, std::shared_ptr<PCA9685> pca){
    Servo::pin = pin;
    Servo::pca9685 = pca;
}
/*!
    this sets the position for the servo in normal mode to pursue
    \param degree 0-360 for the end effector
    \return 0 unless something goes wrong
*/
int Servo::setPosition(int degree, int max){
    double power = (PCA9685_VALUE_MAX - 1) * (degree/max);
    pca9685->Write(CHANNEL(pin), VALUE(power));
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