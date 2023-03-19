#include "LoadMarshmellow.h"
#include "../Robot/mechanisms/revolver.h"
#include "Pringle.h"
#include "DelayAction.h"

LoadMarshmellow::LoadMarshmellow(Revolver::MARSHMALLOWS color){
    LoadMarshmellow::color = color;
    std::string colors[] = {"EMPTY", "RED", "GREEN", "WHITE"};
    name = "Load Marshmelllow (" + colors[color] + ")";
    GoldRushAction::setNext(new Pringle(2));
}

GoldRushAction* LoadMarshmellow::run(Robot* robot){
    return robot->revolver->load_marshmallow(color) ? nextAction : this;
}

