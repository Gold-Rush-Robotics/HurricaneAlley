#include "drivetrain.h"
#include <math.h>
#include <memory>

Drivetrain::Drivetrain(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> eH){
    pca9685 = pca;
    encoderHandler = eH;
    fl = std::make_shared<Motor>(25, 3, pca);
    fr = std::make_shared<Motor>(23, 1, pca);
    bl = std::make_shared<Motor>(24, 2, pca);
    br = std::make_shared<Motor>(18, 0, pca);
    br->reverse();
    fr->reverse();
}


void Drivetrain::drivePow(double forward, double strafe, double turn){
    std::cout << forward << "|" << strafe << "|" << turn << std::endl;
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


// Update Values --Specifically Wheel radius, L and B
static double ODO_R = 1.903190; // Wheel radius in cm
static double ODO_N = 8192;     // Encoder ticks per Revolution
static double CM_PER_TICK = 2.0 * M_PI * ODO_R / ODO_N;

static double ODO_L = 18.225; // Tuneable Distance between Left and Right Encoders
static double ODO_B = 9.5;    // Tuneable Distance between Back Encoder to center of robot (only in the X direction)


void *Drivetrain::encoderThread(void *arguments){
    encoderHandler->resetPositions();
    int32_t posL; // left
    int32_t posR; // right
    int32_t posH; // back
    int32_t oldL; // left
    int32_t oldR; // right
    int32_t oldH; // back
    
    while (true){
        oldL = posL;
        oldR = posR;
        oldH = posH;

        posL = encoderHandler->getPos(0);
        posR = encoderHandler->getPos(1);
        posH = encoderHandler->getPos(2);

        // Delta change in odometers since last loop
        long dR = posR - oldR;
        long dL = posL - oldL;
        long dH = posH - posH;

        // Convert odometer changes to locally referenced position change
        double dT = CM_PER_TICK * (dR - dL) / ODO_L;
        double dX = CM_PER_TICK * (dR + dL) / 2;
        double dY = CM_PER_TICK * (dH + (((dR - dL) / ODO_L) * ODO_B));

        // Takes average heading change so that we get the most accurate change
        double tempH = position.h + (dT / 2.0);

        // Calculate triginometric functions once
        double cosTempH = std::cos(tempH);
        double sinTempH = std::sin(tempH);

        // Convert locally referenced position change to globally referenced position
        position.x += dX * cosTempH - dY * sinTempH;
        position.y += dX * sinTempH + dY * cosTempH;
        position.h += dT;
    }
}
