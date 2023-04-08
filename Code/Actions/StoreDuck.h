#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"
#include "GoToStore1.h"
#include "GoToStore2.h"
#include "GoToAgitator.h"

#ifndef STOREDUCKH
#define STOREDUCKH

class StoreDuck : public GoldRushAction {
    public:
        StoreDuck(double seconds);
        GoldRushAction* run(Robot* robot);
        bool Store1(Robot* robot);
        bool Store2(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
        GoldRushAction* delay4;
        GoToAgitator* ag;
        GoToStore2* go_to_store_2;
        GoToStore1* go_to_store_1;
};

#endif