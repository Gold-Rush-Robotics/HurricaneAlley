#include "GoToAgitator.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

GoToAgitator::GoToAgitator(double seconds){
    name = "Not at Agitator";
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
}

GoldRushAction* GoToAgitator::run(Robot* robot){
    std::cout << "Agitator: " << state << std::endl;
    switch(state)
    {
        case 0:
            // Move the j2
            robot->duck->j2_servo(Duck::JOINT_POSITIONS::AGITATOR);
            state++;
            break;
        case 1:
            // Delay for j2 to get there
            name = "Agitator - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 2:
            // Turn to the storage position
            robot->duck->j1_servo(Duck::JOINT_POSITIONS::AGITATOR);
            state++;
            break;
        case 3:
            name = "Agitator - Delay 2";
            if (delay2->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move the j3
            robot->duck->j3_servo(Duck::JOINT_POSITIONS::AGITATOR);
            state++;
            break;
        case 5:
            // Delay for j3
            name = "Agitator - Delay 3";
            if (delay3->run(robot) == nullptr)
                return nextAction;
            break;
    }
    return this;
}