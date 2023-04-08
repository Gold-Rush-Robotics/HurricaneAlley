#include "Pringle.h"
#include "../Robot/mechanisms/revolver.h"
#include "DelayAction.h"

Pringle::Pringle(double seconds){
    name = "Insert to Pringle";
    state = 0;
    delay1 = new DelayAction(seconds);
    delay2 = new DelayAction(seconds);
    delay3 = new DelayAction(1);
    delay4 = new DelayAction(1);
}
GoldRushAction* Pringle::run(Robot* robot){
    switch(state){
        case 0:
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::ACCEPTING);
            state++;
            break;
        case 1:
            name = "Pringle - Delay 3";
            if(delay3->run(robot) == nullptr){
                state++;
            }
            break;
        case 2:
            robot->revolver->insert_loader();
            state++;
            break;
        case 3:
            name = "Pringle - Delay 1";
            if(delay1->run(robot) == nullptr){
                state++;
            }
            break;
        case 4:
            robot->revolver->pringle_servo(Revolver::PRINGLE_STATES::CLOSED);
            state++;
            break;
        case 5:
            name = "Pringle - Delay 4";
            if(delay4->run(robot) == nullptr){
                state++;
            }
            break;
        case 6:
        
            robot->revolver->retract_loader();
            state++;
            break;
        case 7:
            name = "Pringle - Delay 2";
            if(delay2->run(robot) == nullptr){
                return nextAction;
            }
            break;
    }
    return this;
}