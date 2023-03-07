#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef TURNOFFAGITATORH
#define TURNOFFAGITATORH

class TurnOffAgitator : public GoldRushAction {
    public:
        TurnOffAgitator();
        GoldRushAction* run(Robot* robot);
};

#endif