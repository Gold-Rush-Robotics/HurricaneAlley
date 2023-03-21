#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

#ifndef GOTOSTORE1H
#define GOTOSTORE1H

class GoToStore1 : public GoldRushAction {
    public:
        GoToStore1(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif