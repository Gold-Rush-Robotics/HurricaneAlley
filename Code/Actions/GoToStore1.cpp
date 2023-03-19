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
            robot->duck->shoulder_servo(Duck::SHOULDER_POSITIONS::STORE_1);
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
            robot->duck->waist_servo(Duck::WAIST_POSITIONS::STORE_1);
            state++;
            break;
        case 3:
            name = "Store 1 - Delay 2";
            if (delay2->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move the Elbow
            robot->duck->elbow_servo(Duck::ELBOW_POSITIONS::STORE_1);
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