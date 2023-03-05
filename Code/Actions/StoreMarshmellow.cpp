#include "StoreMarshmellow.h"


StoreMarshmellow::DriveToPointAction(Revolver::Marshmallow color){
        StoreMarshmellow::color = color;
        name = "Load Marshmelllow (" + std::to_string(color) + ")";
    }

GoldRushAction* StoreMarshmellow::run(Robot* robot){
        bool test = robot->revolver->store_marshmallow(color)
        return test ? nextAction : this;
    }