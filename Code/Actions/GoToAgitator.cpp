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
    switch(state)
    {
        case 0:
            // Move the Shoulder
            robot->duck->move_shoulder(1800); // Need to update this
            state++;
            break;
        case 1:
            // Delay for Shoulder to get there
            name = "Agitator - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 2:
            // Turn to the storage position
            robot->duck->turn_to_space_1();
            state++;
            break;
        case 3:
            name = "Agitator - Delay 2";
            if (delay2->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move the Elbow
            robot->duck->move_elbow(1800); // Need to update this
            state++;
            break;
        case 5:
            // Delay for Elbow
            name = "Agitator - Delay 3";
            if (delay3->run(robot) == nullptr)
                state++;
            break;
    }
    return this;
}