#include "../Robot/robot.h"

#ifndef GoldRushActionH
#define GoldRushActionH

class GoldRushAction{
    public:
        void setNext(GoldRushAction *action);
        void setLast(GoldRushAction *action);
        GoldRushAction* run(Robot* robot);
    protected:
        GoldRushAction *nextAction;
};

#endif