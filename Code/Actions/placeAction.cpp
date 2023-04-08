#include "placeAction.h"
#include "../Utilities/pose.h"
#include "DelayAction.h"
#include "../Robot/mechanisms/duck.h"

Pose robotCur;

PlaceAction::PlaceAction(StackAction::StackHeight height){
    name = "Place on " + std::to_string(height) + " Stack";
    size = height;
    state = 0;
    delay1 = new DelayAction(2);
    delay2 = new DelayAction(1);
    delay3 = new DelayAction(1);
    ag = new GoToAgitator(1);
    go_to_stack = new GoToStack(1.0, size);
    go_to_store_1 = new GoToStore1(1);
    go_to_store_2 = new GoToStore2(1);
}
GoldRushAction* PlaceAction::run(Robot* robot){
    std::cout<< state << std::endl;
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
            // Move to correct storage location
            switch (robot->duck->stored_ducks)
            {
                case 3:
                    // Move arm to agitator
                    ag->printName();
                    
                    if (ag->run(robot) == nullptr)
                        state++;
                    break;
                case 2:
                    // Move to Store 2
                    go_to_store_2->printName();
                    if (go_to_store_2->run(robot) == nullptr)
                        state++;
                    break;
                case 1:
                    // Move to Store 1
                    go_to_store_1->printName();
                    if (go_to_store_1->run(robot) == nullptr)
                        state++;
                    break;
                case 0:
                    std::cout << "No stored ducks" << std::endl;
                    // No Stored Ducks Error
                    return nextAction;
            }
            break;
        case 5:
            // Move to Stack
            if (go_to_stack->run(robot) == nullptr)
                state++;
            break;
        case 6:
            // Release Duck
            robot->duck->release_duck();
            state++;
            break;
        case 7:
            // Delay for Releasing Duck
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 8:
            // Raise Arm
            robot->duck->j2_servo(Duck::JOINT_POSITIONS::RAISED);
            state++;
            break;
        case 9:
            // Delay for Raising
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 10:
            // Back up
            if(robot->drivetrain->driveToPoint(new Pose(robotCur.x+10, robotCur.y, robotCur.theta), 1, 0.01)){
                state++;
            }
            break;
        case 11:
            // Raise Pringle
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::CLOSED);
            robot->revolver->drop_servo(false);
            state++;
            break;
        case 12:
            if(delay3->run(robot) == nullptr){
                return nextAction;
            }
            break;
    }
    return this;
}