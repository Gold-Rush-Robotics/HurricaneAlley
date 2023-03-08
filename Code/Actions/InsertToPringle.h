#include "GoldRushAction.h"
#include "../Robot/mechanisms/revolver.h"

#ifndef LOADMARSHMELLOWH
#define LOADMARSHMELLOWH

class InsertToPringle : public GoldRushAction {
    public:
        InsertToPringle(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
};

#endif