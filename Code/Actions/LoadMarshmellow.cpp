#include "LoadMarshmellow.h"
#include "../Robot/mechanisms/revolver.h"


std::string colors[] = {"EMPTY", "RED", "GREEN", "WHITE"};

LoadMarshmellow::LoadMarshmellow(Revolver::MARSHMALLOWS color){
        LoadMarshmellow::color = color;
        name = "Load Marshmelllow (" + colors[color] + ")";
    }

GoldRushAction* LoadMarshmellow::run(Robot* robot){
    return robot->revolver->load_marshmallow(color) ? nextAction : this;
}