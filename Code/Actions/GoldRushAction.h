#include "../Robot/robot.h"

class GoldRushAction{
    public:
        void setNext(GoldRushAction *action);
        void setLast(GoldRushAction *action);
        GoldRushAction* run(Robot robot);
    protected:
        GoldRushAction *nextAction;
};