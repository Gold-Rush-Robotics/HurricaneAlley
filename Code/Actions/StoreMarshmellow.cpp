#include "StoreMarshmellow.h"
#include "DelayAction.h"
#include "TurnOffAgitator.h"
#include <string>

StoreMarshmellow::StoreMarshmellow(MARSHMALLOWS color){
        StoreMarshmellow::color = color;
        std::string colors[] = {"EMPTY", "RED", "GREEN", "WHITE"};
        name = "Store Marshmelllow (" + colors[color] + ")";
        GoldRushAction::setNext(new DelayAction(1.3))->setNext(new TurnOffAgitator());
    }

GoldRushAction* StoreMarshmellow::run(Robot* robot){
        robot->revolver->turn_on_agitator();
        return robot->revolver->store_marshmallow() ? nextAction : this;
    }