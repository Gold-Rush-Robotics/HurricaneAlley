#include <stdint.h>
#include <memory>
#include "utilFunc.h"
#include "bcm2835.h"
#include "pca9685.h"

class Motor{
    private:
        std::shared_ptr<PCA9685> pca9685;
        int directionPin = 0;
        int speedPin = 0;
        bool reversed = false;
    public:
        Motor(int directionPin, int speedPin, std::shared_ptr<PCA9685> pca);
        void reverse();
        bool isReversed();
        void setPower(double power);
};