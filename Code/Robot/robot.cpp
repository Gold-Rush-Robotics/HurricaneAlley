#include "robot.h"

Robot::Robot(){
    // Set up pwm hat connection
    if (getuid() != 0)
    {
        fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
        return -1;
    }

    if (bcm2835_init() != 1)
    {
        fprintf(stderr, "bcm2835_init() failed\n");
        return -2;
    }

    PCA9685 pca9685;
    pca9685.Dump();
    pca9685.SetFrequency(1000);

    drivetrain = new Drivetrain(pca9685);
}


Robot::driveController(double s1y, double s1x, double s2y, double s2x, double multiplier){
    drivetrain.drivePow(multiplier * s1y, multiplier * s1x, multiplier * -s2x);
}