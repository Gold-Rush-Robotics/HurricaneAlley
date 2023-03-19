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
            robot->duck->shoulder_servo(Duck::SHOULDER_POSITIONS::AGITATOR);
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
            robot->duck->waist_servo(Duck::WAIST_POSITIONS::AGITATOR);
            state++;
            break;
        case 3:
            name = "Agitator - Delay 2";
            if (delay2->run(robot) == nullptr)
                state++;
            break;
        case 4:
            // Move the Elbow
            robot->duck->elbow_servo(Duck::ELBOW_POSITIONS::AGITATOR);
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