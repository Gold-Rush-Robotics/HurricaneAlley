#include "DriveToPointAction.h"


DriveToPointAction::DriveToPointAction(Pose* point, double distanceTolerance, double angleTolerance){
        DriveToPointAction::point = point;
        disT = distanceTolerance;
        angT = angleTolerance;
        name = "Drive to Point (" + std::to_string(point->x) + " , " + std::to_string(point->y) + ")";
    }

GoldRushAction* DriveToPointAction::run(Robot* robot){
        bool test = robot->drivetrain->driveToPoint(point, disT, angT);
        return test ? nextAction : this;
    }