#include "InsertToPringle.h"
#include "../Robot/mechanisms/revolver.h"
#include "DelayAction.h"

InsertToPringle::InsertToPringle(double seconds){
    name = "Insert to Pringle";
    state = 0;
    InsertToPringle::delay1 = new DelayAction(seconds);
    InsertToPringle::delay2 = new DelayAction(seconds);
}
GoldRushAction* InsertToPringle::run(Robot* robot){
    switch(state){
        case 0:
            robot->revolver->insert_loader();
            state++;
            break;
        case 1:
            if(delay1->run(robot) == nullptr){
                state++;
            }
            break;
        case 2:
            robot->revolver->retract_loader();
            state++;
            break;
        case 3:
            if(delay2->run(robot) == nullptr){
                return nextAction;
            }
            break;
    }
    return this;
}