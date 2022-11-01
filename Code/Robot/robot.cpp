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


/**
Robot::Robot(){
    // Set up pwm hat connection
    std::cout << "robot init" << std::endl;
}
*/
void Robot::start(){
    if (bcm2835_init() != 1)
    {
        fprintf(stderr, "bcm2835_init() failed\n");
    }
    pca9685 = new PCA9685();
    pca9685->Dump();
    pca9685->SetFrequency(1000);
    drivetrain->init(pca9685);
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

void Drivetrain::init(PCA9685* pca){
    std::cout << "AAAA" << std::endl;
    Drivetrain::pca9685 = pca;
    std::cout << "AAAA" << std::endl;
    Drivetrain::fl.init(4, 0);
    Drivetrain::fr.init(17, 1);
    Drivetrain::bl.init(22, 3);
    Drivetrain::br.init(27, 2);
    fl.reverse();
    bl.reverse();
    br.reverse();
}


void Drivetrain::drivePow(double forward, double strafe, double turn){

    std::cout << forward << "|" << strafe << "|" << turn << std::endl;

    double powerFL = forward + strafe - turn;
    double powerFR = forward - strafe + turn;
    double powerBL = forward - strafe - turn;
    double powerBR = forward + strafe + turn;

    std::cout << powerFL << "|" << powerFR << "|" << powerBL << "|" << powerBR << std::endl;

    double powerMax = std::max(
        abs(powerFL), 
        std::max(
            abs(powerFR), 
            std::max(
                abs(powerBL), 
                abs(powerBR))));

    std::cout << powerMax << std::endl;

    if (powerMax > 1.0)
    {
        powerFL /= powerMax;
        powerFR /= powerMax;
        powerBL /= powerMax;
        powerBR /= powerMax;
    }

    powerFL = constrain(-1.0, 1.0, powerFL);
    powerFR = constrain(-1.0, 1.0, powerFR);
    powerBL = constrain(-1.0, 1.0, powerBL);
    powerBR = constrain(-1.0, 1.0, powerBR);

    printf("\n\n\nfl: %.2f fr: %.2f\n\nbl: %.2f br: %.2f\n", powerFL, powerFR, powerBL, powerBR);

    driveM(powerFL, powerFR, powerBL, powerBR);
}


void Drivetrain::driveM(double fl, double fr, double bl, double br){

    Drivetrain::fl.setPower(fl, Drivetrain::pca9685);
    Drivetrain::fr.setPower(fr, Drivetrain::pca9685);
    Drivetrain::bl.setPower(bl, Drivetrain::pca9685);
    Drivetrain::br.setPower(br, Drivetrain::pca9685);
}

void Drivetrain::stop(){
    driveM(0,0,0,0);
}
