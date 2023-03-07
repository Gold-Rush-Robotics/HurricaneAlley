#include "StoreMarshmellow.h"
#include <string>

std::string colors[] = {"EMPTY", "RED", "GREEN", "WHITE"};

StoreMarshmellow::StoreMarshmellow(Revolver::MARSHMALLOWS color){
        StoreMarshmellow::color = color;
        name = "Load Marshmelllow (" + colors[color] + ")";
    }

GoldRushAction* StoreMarshmellow::run(Robot* robot){
        return robot->revolver->store_marshmallow(color) ? nextAction : this;
    }