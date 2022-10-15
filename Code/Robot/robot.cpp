#include "robot.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <algorithm>
#include <softPwm.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <cstdio>
#include <cstdint>
#include <signal.h>

Robot::Robot(){
    // Set up pwm hat connection
    if (bcm2835_init() != 1)
    {
        fprintf(stderr, "bcm2835_init() failed\n");
    }

    PCA9685 pca9685;
    pca9685.Dump();
    pca9685.SetFrequency(1000);

    drivetrain = Drivetrain(pca9685);
}


void Robot::driveController(double s1y, double s1x, double s2y, double s2x, double multiplier){
    drivetrain.drivePow(multiplier * s1y, multiplier * s1x, multiplier * -s2x);
}

void Robot::stop(){
    drivetrain.stop();
}