#include "drivetrain.h"
#include <algorithm>
#include <iostream>


// Pink zip tie is front, Purple zip tie is back
enum motorNums
{
    motorFL = 0,
    motorFR = 1,
    motorBL = 2,
    motorBR = 3
};

void Drivetrain::init(PCA9685 pca9685){
    Drivetrain::fl.init(4, 0, pca9685);
    Drivetrain::fr.init(17, 1, pca9685);
    Drivetrain::bl.init(22, 3, pca9685);
    Drivetrain::br.init(27, 2, pca9685);
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
    Drivetrain::fl.setPower(fl);
    Drivetrain::fr.setPower(fr);
    Drivetrain::bl.setPower(bl);
    Drivetrain::br.setPower(br);
}

void Drivetrain::stop(){
    driveM(0,0,0,0);
}
