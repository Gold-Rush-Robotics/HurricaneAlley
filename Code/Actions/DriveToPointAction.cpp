#include "GoldRushAction.h"
#include "../Utilities/pose.h"

class DriveToPointAction : GoldRushAction {
    Pose point;
    double disT;
    double angT;

    DriveToPointAction(Pose point, double distanceTolerance, double angleTolerance){
        DriveToPointAction::point = point;
        disT = distanceTolerance;
        angT = angleTolerance;
    }

    GoldRushAction* run(Robot robot){
        bool test = robot.drivetrain->driveToPoint(point, disT, angT);
        return test ? nextAction : this;
    }

};