#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

#ifndef GOTOSTACKH
#define GOTOSTACKH

class GoToStack : public GoldRushAction {
    public:
        GoToStack(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif