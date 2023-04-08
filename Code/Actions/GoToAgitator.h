#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

#ifndef GOTOAGITATORH
#define GOTOAGITATORH

class GoToAgitator : public GoldRushAction {
    public:
        GoToAgitator(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif