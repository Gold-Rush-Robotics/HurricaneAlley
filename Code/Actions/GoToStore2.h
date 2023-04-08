#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

#ifndef GOTOSTORE2H
#define GOTOSTORE2H

class GoToStore2 : public GoldRushAction {
    public:
        GoToStore2(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif