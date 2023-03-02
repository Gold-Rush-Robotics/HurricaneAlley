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
    position.update(0,0,0);
    translatePID = PID(0.1, 1.0, -1.0, 1.0, 0.0, 0.0);
    rotatePID = PID(0.1, 1.0, -1.0, 1.0, 0.0, 0.0);
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

static double ODO_L = 23.6; // Tuneable Distance between Left and Right Encoders
static double ODO_B = 11.5;    // Tuneable Distance between Back Encoder to center of robot (only in the X direction)

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
        posR = -e[1];
        posH = e[2];

        // Delta change in odometers since last loop
        long dR = posR - oldR;
        long dL = posL - oldL;
        long dH = posH - oldH;

        

        // Convert odometer changes to locally referenced position change
        double fi = CM_PER_TICK * (dR - dL) / ODO_L;
        double dXC = CM_PER_TICK * (dR + dL) / 2;
        double dXh = CM_PER_TICK * (dH + (fi/CM_PER_TICK * ODO_B));

        Eigen::Vector3d v(dXC, dXh, fi);

        std::cout << dH << " " << posH << " " << oldH << std::endl;

        std::cout << dXh << " " << dH << " " << (fi/CM_PER_TICK * ODO_B) << " " << (dH + (fi/CM_PER_TICK * ODO_B)) << std::endl;

        // https://gm0.org/en/latest/docs/software/concepts/odometry.html
        // this is just for rotating it by the heading at the begining 
        Eigen::Matrix3d m1 {
            {std::cos(position.theta), -std::sin(position.theta), 0},
            {std::sin(position.theta), std::cos(position.theta),  0},
            {0,                     0,                    1}
        };

        double sineTerm = 0;
        double cosTerm = 0;

        //avoid a devide by 0 erorr with an approximation (similar to l'hopital)
        if(essentiallyEqual(fi, 0, 0.0001)){
           sineTerm = 1.0 - fi * fi / 6.0;
           cosTerm = fi / 2.0;
        } else { 
            sineTerm = std::sin(fi)/fi;
            cosTerm = (1-std::cos(fi))/fi;
        }


        //this is a funky rotation to treat the vector as having moved along a curve
        // sine term and cos term are defined above
        Eigen::Matrix3d m2 {
            {sineTerm, -cosTerm, 0},
            {cosTerm , sineTerm, 0},
            {0       , 0       , 1}
        };

        Eigen::Vector3d v2 = m1 * m2 * v;

        position.update(v2(0), v2(1), v2(2));
}

void Drivetrain::printPosition(){
    std::cout << position.x << " - " << position.y << " - " << position.theta << std::endl;
}

bool Drivetrain::driveToPoint(Pose p, double distanceTolerance, double headingTolerance){
    bool inDistance = std::abs(position.getDistance(p)) <= distanceTolerance;
    bool inHeading = std::abs(position.getHeadingOffset(p)) <= headingTolerance;
    double xComp = 0;
    double yComp = 0;
    double hComp = 0;
    if(!inDistance){
        double correction = translatePID.calculate(0, position.getDistance(p));
        double heading = position.getDirection(p);
        xComp += correction * std::sin(heading);
        yComp += correction * std::cos(heading);
    }

    if(!inHeading){
        double correction = rotatePID.calculate(0, position.getHeadingOffset(p));
        hComp += correction;
    }

    drivePow(xComp, yComp, hComp);
    return inDistance && inHeading;
}
