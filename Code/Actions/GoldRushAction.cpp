#include "GoldRushAction.h"

void GoldRushAction::setLast(GoldRushAction *action){
    if(nextAction == nullptr){
        nextAction = action;
    } else {
        nextAction->setNext(action);
    }
}

GoldRushAction* GoldRushAction::setNext(GoldRushAction *action){
    if(nextAction == nullptr){
        nextAction = action;
    } else {
        action->setNext(nextAction);
        nextAction = action;
    }
    return action;
}

GoldRushAction* GoldRushAction::run(Robot* robot){
    return this;
}

void GoldRushAction::printName(){
    std::cout << name << std::endl;
}
