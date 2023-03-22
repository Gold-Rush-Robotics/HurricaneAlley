#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

#ifndef GOTOSTACKH
#define GOTOSTACKH

class GoToStack : public GoldRushAction {
    public:
        GoToStack(double seconds, int height);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        int height;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
};

#endif