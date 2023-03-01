#include "drivetrain.h"
#include <math.h>
#include <memory>
#include <Eigen/Dense>


Drivetrain::Drivetrain(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> eH){
    pca9685 = pca;
    encoderHandler = eH;
    fl = std::make_shared<Motor>(25, 3, pca);
    fr = std::make_shared<Motor>(23, 1, pca);
    bl = std::make_shared<Motor>(24, 2, pca);
    br = std::make_shared<Motor>(18, 0, pca);
    br->reverse();
    fr->reverse();
    encoderHandler->resetPositions();
    position.x = 0;
    position.y = 0;
    position.h = 0;
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

int32_t posL = 0; // left
int32_t posR = 0; // right
int32_t posH = 0; // back
int32_t oldL = 0; // left
int32_t oldR = 0; // right
int32_t oldH = 0; // back

void Drivetrain::encoderLogic(){
        oldL = posL;
        oldR = posR;
        oldH = posH;
        int32_t* e = encoderHandler->getValues();
        posL = e[0];
        posR = e[1];
        posH = e[2];

        // Delta change in odometers since last loop
        long dR = posR - oldR;
        long dL = posL - oldL;
        long dH = posH - posH;

        // Convert odometer changes to locally referenced position change
        double fi = CM_PER_TICK * (dR - dL) / ODO_L;
        double dXC = CM_PER_TICK * (dR + dL) / 2;
        double dXh = CM_PER_TICK * (dH + (((dR - dL) / ODO_L) * ODO_B));

        Eigen::Vector3d v(dXC, dXh, fi);

        

        // https://gm0.org/en/latest/docs/software/concepts/odometry.html
        // this is just for rotating it by the heading at the begining 
        Eigen::Matrix3d m1 {
            {std::cos(position.h), -std::sin(position.h), 0},
            {std::sin(position.h), std::cos(position.h),  0},
            {0,                     0,                    1}
        };
        //this is a funky rotation to treat the vector as having moved along a curve
        Eigen::Matrix3d m2 {
            {std::sin(fi)/fi, (std::cos(fi)-1)/fi, 0},
            {(1-std::cos(fi))/fi, std::sin(fi)/fi, 0},
            {0,                     0,              1}
        };

        Eigen::Vector3d v2 = m1 * m2 * v;

        std::cout << "V:"  << v << std::endl;
        std::cout << "M1:" << m1 << std::endl;
        std::cout << "M2:" << m2 << std::endl;
        std::cout << "V2:" << v2 << std::endl;

        position.x += v2(0);
        position.y += v2(1);
        position.h += v2(2);
}

void Drivetrain::printPosition(){
    std::cout << position.x << " - " << position.y << " - " << position.h << std::endl;
}