#include "drivetrain.h"


// Pink zip tie is front, Purple zip tie is back
enum motorNums
{
    motorFL = 0,
    motorFR = 1,
    motorBL = 2,
    motorBR = 3
};


Drivetrain::Drivetrain(PCA9685 pca9685){
    Drivetrain::pca9685 = pca9685;
    Drivetrain::motors = new Motor[Motor(4, 0, pca9685), Motor(17,1,pca9685), Motor(22,3,pca9685), Motor(27,2,pca9685)];
    motors[0].reverse();
    motors[2].reverse();
    motors[3].reverse();
}

Drivetrain::drivePow(double forward, double strafe, double turn){
    double powerFL = forward + strafe - turn;
    double powerFR = forward - strafe + turn;
    double powerBL = forward - strafe - turn;
    double powerBR = forward + strafe + turn;

    double powerMax = max(abs(powerFL), max(abs(powerFR), max(abs(powerBL), abs(powerBR))));

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

    // printf("\n\n\nfl: %.2f fr: %.2f\n\nbl: %.2f br: %.2f\n", powerFL, powerFR, powerBL, powerBR);

    driveM(powerFL, powerFR, powerBL, powerBR);
}


Drivetrain::driveM(double fl, double fr, double bl, double br){
    motors[motorFL].setPower(fl);
    motors[motorFR].setPower(fr);
    motors[motorBL].setPower(bl);
    motors[motorBR].setPower(br);
}
