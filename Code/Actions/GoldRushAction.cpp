#include "GoldRushAction.h"

void GoldRushAction::setLast(GoldRushAction action){
    if(nextAction == nullptr){
        nextAction = action;
    } else {
        nextAction.setNext(action);
    }
}

void GoldRushAction::setNext(GoldRushAction action){
    if(nextAction == nullptr){
        nextAction = action;
    } else {
        action.setNext(nextAction);
        nextAction = action;
    }
}

GoldRushAction GoldRushAction::run(){
    return nextAction;
}