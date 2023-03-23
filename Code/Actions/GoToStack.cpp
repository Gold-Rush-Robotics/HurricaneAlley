#include "GoToStack.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

GoToStack::GoToStack(double seconds, int height){
    name = "Not at Stack";
    state = 0;
    this->height = height;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
}

GoldRushAction* GoToStack::run(Robot* robot){
    switch(state)
    {
        case 0:
            // Move the j2
            robot->duck->j2_servo(Duck::JOINT_POSITIONS::STACK);
            state++;
            break;
        case 1:
            // Delay for j2 to get there
            name = "Stack 1 - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 2:
            // Turn to the storage position
            robot->duck->j1_servo(Duck::JOINT_POSITIONS::STACK);
            state++;
            break;
        case 3:
            name = "Stack 1 - Delay 2";
            if (delay2->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move the j3
            if (height == 3)
                robot->duck->j3_servo(Duck::JOINT_POSITIONS::STACK_3);
            else if (height == 2)
                robot->duck->j3_servo(Duck::JOINT_POSITIONS::STACK_2);
            state++;
            break;
        case 5:
            // Delay for j3
            name = "Stack 1 - Delay 3";
            if (delay3->run(robot) == nullptr)
                return nextAction;
            break;
    }
    return this;
}