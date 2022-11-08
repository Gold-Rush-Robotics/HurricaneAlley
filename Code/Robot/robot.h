#include <iostream>
#include "../HardwareInterfaces/hardware.h"
#include "bcm2835.h"
#include "pca9685.h"
#include "drivetrain.h"

class Robot{
    public:
        std::shared_ptr<PCA9685> pca9685;
        void start();
        void driveController(double s1y, double s1x, double s2y, double s2x, double multiplier);
        void stop();
    private:
        std::shared_ptr<Drivetrain> drivetrain;
};