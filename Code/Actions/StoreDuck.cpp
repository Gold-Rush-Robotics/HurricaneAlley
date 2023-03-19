#include "StoreDuck.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

StoreDuck::StoreDuck(double seconds){
    name = "Storing Duck " + (Duck::stored_ducks + 1);
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
}

GoldRushAction* StoreDuck::run(Robot* robot){
    if (robot->duck->stored_ducks == 0)
        return StoreDuck::Store1(robot);
    else if (robot->duck->stored_ducks == 1)
        return StoreDuck::Store2(robot);
    else
        robot->duck->stored_ducks++;
    return nextAction;
}

GoldRushAction* StoreDuck::Store1(Robot* robot){
    switch(state)
    {
        case 0:
            // Move to Agitator
            GoToAgitator::run(robot);
            state++;
            break;
        case 1:
            // Grip duck
            robot->duck->grip_duck();
            state++;
            break;
        case 2:
            // Move Elbow Up
            robot->duck->elbow_servo(Duck::ELBOW_POSITIONS::RAISED);
            state++;
            break;
        case 3:
            // Delay for Elbow
            name = "Store - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move to Store 1
            GoToStore1::run(robot); // GoToStore1 isn't recognized in vs code for some reason
            state++;
            break;
        case 5:
            // Release Gripper
            robot->duck->release_duck();
            state++;
            robot->duck->stored_ducks++;
            break;
    }
}

GoldRushAction* StoreDuck::Store2(Robot* robot){
    switch(state)
    {
        case 0:
            // Move to Agitator
            GoToAgitator::run(robot);
            state++;
            break;
        case 1:
            // Grip duck
            robot->duck->grip_duck();
            state++;
            break;
        case 2:
            // Move Elbow Up
            robot->duck->elbow_servo(Duck::ELBOW_POSITIONS::RAISED);
            state++;
            break;
        case 3:
            // Delay for Elbow
            name = "Store - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move to Store 2
            GoToStore2::run(robot);
            state++;
            break;
        case 5:
            // Release Gripper
            robot->duck->release_duck();
            state++;
            robot->duck->stored_ducks++;
            break;
    }
}