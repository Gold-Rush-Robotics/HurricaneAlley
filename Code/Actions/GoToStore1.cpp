#include "GoToStore1.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

GoToStore1::GoToStore1(double seconds){
    name = "Not at Store 1";
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
}

GoldRushAction* GoToStore1::run(Robot* robot){
    switch(state)
    {
        case 0:
            // Move the Shoulder
            robot->duck->move_shoulder(1800); // Need to update this
            state++;
            break;
        case 1:
            // Delay for Shoulder to get there
            name = "Store 1 - Delay 1";
            if (delay1->run(robot) == nullptr)
                state++;
            break;
        case 2:
            // Turn to the storage position
            robot->duck->turn_to_space_1();
            state++;
            break;
        case 3:
            name = "Store 1 - Delay 2";
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
            name = "Store 1 - Delay 3";
            if (delay3->run(robot) == nullptr)
                state++;
            break;
    }
    return this;
}