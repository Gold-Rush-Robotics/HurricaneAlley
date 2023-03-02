#include "DriveToPointAction.h"


DriveToPointAction::DriveToPointAction(Pose* point, double distanceTolerance, double angleTolerance){
        DriveToPointAction::point = point;
        disT = distanceTolerance;
        angT = angleTolerance;
    }

GoldRushAction* DriveToPointAction::run(Robot* robot){
        bool test = robot->drivetrain->driveToPoint(point, disT, angT);
        return test ? nextAction : this;
    }