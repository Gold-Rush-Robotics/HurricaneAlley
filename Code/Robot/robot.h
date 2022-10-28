#include <iostream>
#include "HardwareInterfaces/hardware.h"
#include "bcm2835.h"
#include "pca9685.h"

class Drivetrain{
    public:
        Motor fl;
        Motor fr;
        Motor bl;
        Motor br;
        PCA9685* pca9685;
        void init();
        void drivePow(double forward, double strafe, double turn);
        void drive(double dx, double dy, double dTheta);
        void stop();
    private:
        void driveM(double fl, double fr, double bl, double br);
};

class Robot{
    public:
        static PCA9685* pca9685;
        void start();
        void driveController(double s1y, double s1x, double s2y, double s2x, double multiplier);
        void stop();
    private:
        Drivetrain* drivetrain;
};