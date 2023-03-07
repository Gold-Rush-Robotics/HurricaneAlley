#include "TurnOffAgitator.h"

TurnOffAgitator::TurnOffAgitator(){
    name = "turn off agitator"
}

GoldRushAction* TurnOffAgitator::run(Robot* robot){
    robot->revolver->turn_off_agitator();
    return nextAction;
}