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
#include <iostream>
#include <memory>

void Robot::start(){
    if (bcm2835_init() != 1)
    {
        fprintf(stderr, "bcm2835_init() failed\n");
    }
    pca9685 = std::make_shared<PCA9685>();
    pca9685->Dump();
    pca9685->SetFrequency(1000);
    drivetrain = std::make_shared<Drivetrain>(pca9685);

}

void Robot::driveController(double s1y, double s1x, double s2y, double s2x, double multiplier){
    std::cout << "Ly:" << s1y << " |Lx:" << s1x << " |Ry:" << s2y << " |Rx:" << s2x << std::endl;
    drivetrain->drivePow(multiplier * s1y, multiplier * s1x, multiplier * -s2x);
}

void Robot::stop(){
    drivetrain->stop();
}

// Pink zip tie is front, Purple zip tie is back
enum motorNums
{
    motorFL = 0,
    motorFR = 1,
    motorBL = 2,
    motorBR = 3
};
