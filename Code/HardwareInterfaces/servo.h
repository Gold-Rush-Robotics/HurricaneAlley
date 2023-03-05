#include "pca9685.h"
#include <memory>

#ifndef SERVOH
#define SERVOH

class Servo{
    public:
        Servo(int pin, std::shared_ptr<PCA9685> pca);
        int setSpeed(double speed);
        int setPosition(double degree, double max);
    private:
        std::shared_ptr<PCA9685> pca9685;
        int pin;
};

#endif