#include "StackAction.h"

StackAction::StackAction(StackHeight height){
    name = "Stack " + std::to_string(height) + " high";
}

GoldRushAction* StackAction::run(Robot* robot){
    return nextAction;
}