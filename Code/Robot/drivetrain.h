#include "HardwareInterfaces/hardware.h"

#include "bcm2835.h"
#include "pca9685.h"


class Drivetrain{
    public:
        Motor fl;
        Motor fr;
        Motor bl;
        Motor br;
        void init(PCA9685 pca9685);
        void drivePow(double forward, double strafe, double turn);
        void drive(double dx, double dy, double dTheta);
        void stop();


    private:
        PCA9685 pca9685;
        void driveM(double fl, double fr, double bl, double br);
};