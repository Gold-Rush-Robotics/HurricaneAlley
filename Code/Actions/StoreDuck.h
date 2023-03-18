#include "GoldRushAction.h"
#include "../Robot/mechanisms/duck.h"
#include "GoToStore1.h"
#include "GoToStore2.h"
#include "GoToAgitator.h"

class StoreDuck : public GoldRushAction {
    public:
        StoreDuck(double seconds);
        GoldRushAction* run(Robot* robot);
        GoldRushAction* Store1(Robot* robot);
        GoldRushAction* Store2(Robot* robot);
    private:
        int state;
        GoldRushAction* delay1;
        GoldRushAction* delay2;
        GoldRushAction* delay3;
        GoldRushAction* delay4;


};