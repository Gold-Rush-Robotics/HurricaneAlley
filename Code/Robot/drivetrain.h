#include <iostream>
#include "../HardwareInterfaces/motor.h"
#include "bcm2835.h"
#include "pca9685.h"

class Drivetrain{
    public:
        Drivetrain(std::shared_ptr<PCA9685> pca);
        void drivePow(double forward, double strafe, double turn);
        void drive(double dx, double dy, double dTheta);
        void stop();
    private:
        std::shared_ptr<Motor> fl, fr, bl, br;
        std::shared_ptr<PCA9685> pca9685;
        void driveM(double fl, double fr, double bl, double br);
};