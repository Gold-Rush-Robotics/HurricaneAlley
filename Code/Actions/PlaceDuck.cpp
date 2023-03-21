#include "PlaceDuck.h"
#include "../Robot/mechanisms/duck.h"
#include "DelayAction.h"

// Call this inside of placeAction
PlaceDuck::PlaceDuck(double seconds, int height){
    name = "Storing Duck " + (Duck::stored_ducks + 1);
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(seconds);
    ag = new GoToAgitator(seconds);
    go_to_store_1 = new GoToStore1(seconds);
    go_to_store_2 = new GoToStore2(seconds);
    go_to_stack = new GoToStack(seconds);
}

GoldRushAction* PlaceDuck::run(Robot* robot){

    switch(robot->duck->stored_ducks)
    {
        case 3:
            ag = ag->run(robot);
            if(ag == nullptr)

            break;
        case 2:
            if ()
    }

    
    return this;
}

