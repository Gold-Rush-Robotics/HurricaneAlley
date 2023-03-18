#include "GoToStore2.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

GoToStore2::GoToStore2(double seconds){
    name = "Not at Store 2";
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
}

GoldRushAction* GoToStore2::run(Robot* robot){
    switch(state)
    {
        case 0:
            // Move the Shoulder
            robot->duck->move_shoulder(1800); // Need to update this
            state++;
            break;
        case 1:
            // Delay for Shoulder to get there
            name = "Store 2 - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 2:
            // Turn to the storage position
            robot->duck->turn_to_space_1();
            state++;
            break;
        case 3:
            name = "Store 2 - Delay 2";
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
            name = "Store 2 - Delay 3";
            if (delay3->run(robot) == nullptr)
                state++;
            break;
    }
    return this;
}