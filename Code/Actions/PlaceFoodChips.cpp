#include "PlaceFoodChips.h"
#include "LoadMarshmellow.h"
#include "DumpPringle.h"

PlaceFoodChips::PlaceFoodChips(CHIPCOLORS color){
    name = "Place " + std::to_string(color) + " chips";
    if(color == CHIPCOLORS::RED){
        this->setLast(new LoadMarshmellow(MARSHMALLOWS::RED_CHIPS))
            ->setLast(new DumpPringle())
    } else {
        this->setLast(new LoadMarshmellow(MARSHMALLOWS::GREEN_CHIPS))
            ->setLast(new DumpPringle())
    }
    
}

GoldRushAction* PlaceFoodChips::run(Robot* robot){
    return nextAction;
}


// Three tall Statue: base level – white, second level – green, third level – red
// with a pink duck on top in the middle of the pond
// Two Tall Statue: base level – white, second level – green
// with a yellow duck on top on the outside pond locations