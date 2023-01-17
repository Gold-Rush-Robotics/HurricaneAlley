#include "pca9685.h"
#include <memory>

class Servo{
    public:
        Servo(int pin, std::shared_ptr<PCA9685> pca);
        int setSpeed(double speed);
        int setPosition(int degree, int max);
    private:
        std::shared_ptr<PCA9685> pca9685;
        int pin;
}