#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef PRINGLEH
#define PRINGLEH

class Pringle : public GoldRushAction {
    public:
        Pringle(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
        GoldRushAction* delay4;
};

#endif