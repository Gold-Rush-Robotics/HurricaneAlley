#include "HardwareInterfaces/motor.h"
#include "HardwareInterfaces/encoder.h"

#include "bcm2835.h"
#include "pca9685.h"


class Drivetrain{
    public:
        Drivetrain(PCA9685 pca9685);
        Drivetrain();
        void drivePow(double forward, double strafe, double turn);
        void drive(double dx, double dy, double dTheta);
        void stop();


    private:
        Motor motors[4];
        Encoder encoders[3];
        PCA9685 pca9685;
        void driveM(double fl, double fr, double bl, double br);
};