#include "StoreDuck.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

StoreDuck::StoreDuck(double seconds){
    name = "Not at Agitator";
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
    return nextAction;
}

GoldRushAction* StoreDuck::Store1(Robot* robot){
    switch(state)
    {
        case 0:
            // Move to Agitator
            state++;
            break;
        case 1:
            // Grip duck
            state++;
            break;
        case 2:
            // Move Elbow/Shoulder Up
            state++;
            break;
        case 3:
            // Delay for Elbow
            state++;
            break;
        case 4:
            // Move to Store 1
            state++;
            break;
        case 5:
            // Release Gripper
            state++;
            break;
    }
}