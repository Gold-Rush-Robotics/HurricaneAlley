#include "placeAction.h"
#include "../Utilities/pose.h"
#include "DelayAction.h"

Pose robotCur;

PlaceAction::PlaceAction(){
    name = "Place Stack";
    state = 0;
    delay1 = new DelayAction(2);
    delay2 = new DelayAction(1);
    delay3 = new DelayAction(1);
}
GoldRushAction* PlaceAction::run(Robot* robot){
    switch(state){
        case 0:
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::CLOSED);
            robot->revolver->drop_servo(true);
            state++;
            break;
        case 1:
            if(delay1->run(robot) == nullptr){
                state++;
            }
            break;
        case 2:
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::OPEN);
            state++;
            break;
        case 3:
            if(delay2->run(robot) == nullptr){
                robotCur = robot->drivetrain->position;
                state++;
            }
            break;
        case 4:
            if(robot->drivetrain->driveToPoint(new Pose(robotCur.x+10, robotCur.y, robotCur.theta), 1, 0.01)){
                state++;
            }
            break;
        case 5:
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::CLOSED);
            robot->revolver->drop_servo(false);
            state++;
            break;
        case 6:
            if(delay3->run(robot) == nullptr){
                return nextAction;
            }
            break;
    }
    return this;
}