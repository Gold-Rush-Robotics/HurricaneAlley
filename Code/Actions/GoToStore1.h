#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"

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