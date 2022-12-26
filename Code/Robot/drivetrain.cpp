#include "drivetrain.h"
#include <memory>

Drivetrain::Drivetrain(std::shared_ptr<PCA9685> pca){
    pca9685 = pca;
    fl = std::make_shared<Motor>(4, 0, pca);
    fr = std::make_shared<Motor>(17, 1, pca);
    bl = std::make_shared<Motor>(22, 3, pca);
    br = std::make_shared<Motor>(27, 2, pca);
    fr->reverse();
}


void Drivetrain::drivePow(double forward, double strafe, double turn){
    //std::cout << forward << "|" << strafe << "|" << turn << std::endl;
    double powerFL = forward + strafe - turn;
    double powerFR = forward - strafe + turn;
    double powerBL = forward - strafe - turn;
    double powerBR = forward + strafe + turn;
    double powerMax = std::max(abs(powerFL), std::max(abs(powerFR), std::max(abs(powerBL), abs(powerBR))));

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

    //printf("\nfl: %.2f fr: %.2f\nbl: %.2f br: %.2f\n", powerFL, powerFR, powerBL, powerBR);

    driveM(powerFL, powerFR, powerBL, powerBR);
}


void Drivetrain::driveM(double fl, double fr, double bl, double br){
    Drivetrain::fl->setPower(fl);
    Drivetrain::fr->setPower(fr);
    Drivetrain::bl->setPower(bl);
    Drivetrain::br->setPower(br);
}

void Drivetrain::stop(){
    driveM(0,0,0,0);
}
