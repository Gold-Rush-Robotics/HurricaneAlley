#include "StoreDuck.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

StoreDuck::StoreDuck(double seconds){
    name = "Storing Duck " + (Duck::stored_ducks + 1);
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
    ag = new GoToAgitator(seconds);
    go_to_store_1 = new GoToStore1(seconds);
    go_to_store_2 = new GoToStore2(seconds);
}

GoldRushAction* StoreDuck::run(Robot* robot){
    if (robot->duck->stored_ducks == 0)
    {
        if (Store1(robot))
            return nextAction;
    }
    else if (robot->duck->stored_ducks == 1)
    {
        if (Store2(robot))
            return nextAction;
    }
    else
    {
        robot->duck->stored_ducks++;
        return nextAction;
    }
    return this;
}

bool StoreDuck::Store1(Robot* robot){
    switch(state)
    {
        case 0:
            // Move to Agitator
            if (ag->run(robot) == nullptr)
                state++;
            break;
        case 1:
            // Grip duck
            robot->duck->grip_duck();
            state++;
            break;
        case 2:
            // Move j3 Up
            robot->duck->j3_servo(Duck::J3_POSITIONS::RAISED);
            state++;
            break;
        case 3:
            // Delay for j3
            name = "Store - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move to Store 1
            if (go_to_store_1->run(robot) == nullptr)
                state++;
            break;
        case 5:
            // Release Gripper
            robot->duck->release_duck();
            state++;
            robot->duck->stored_ducks++;
            return false;
            break;
    }
    return true;
}

bool StoreDuck::Store2(Robot* robot){
    switch(state)
    {
        case 0:
            // Move to Agitator
            if (ag->run(robot) == nullptr)
                state++;
            break;
        case 1:
            // Grip duck
            robot->duck->grip_duck();
            state++;
            break;
        case 2:
            // Move j3 Up
            robot->duck->j3_servo(Duck::J3_POSITIONS::RAISED);
            state++;
            break;
        case 3:
            // Delay for j3
            name = "Store - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move to Store 2
            if (go_to_store_2->run(robot) == nullptr)
                state++;
            break;
        case 5:
            // Release Gripper
            robot->duck->release_duck();
            state++;
            robot->duck->stored_ducks++;
            return false;
            break;
    }
    return true;
}