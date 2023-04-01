#include "StackAction.h"
#include "LoadMarshmellow.h"

StackAction::StackAction(StackHeight height){
    name = "Stack " + std::to_string(height) + " high";
    if(height == 3){
        this->setLast(new LoadMarshmellow(MARSHMALLOWS::RED))
            ->setLast(new LoadMarshmellow(MARSHMALLOWS::GREEN))
            ->setLast(new LoadMarshmellow(MARSHMALLOWS::WHITE));
    } else {
        this->setLast(new LoadMarshmellow(MARSHMALLOWS::GREEN))
            ->setLast(new LoadMarshmellow(MARSHMALLOWS::WHITE));
    }
    
}

GoldRushAction* StackAction::run(Robot* robot){
    return nextAction;
}


// Three tall Statue: base level – white, second level – green, third level – red
// with a pink duck on top in the middle of the pond
// Two Tall Statue: base level – white, second level – green
// with a yellow duck on top on the outside pond locations