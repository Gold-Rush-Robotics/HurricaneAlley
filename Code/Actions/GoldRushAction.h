#include "../Robot/robot.h"
#include <string>

#ifndef GoldRushActionH
#define GoldRushActionH

class GoldRushAction{
    public:
        GoldRushAction* setNext(GoldRushAction *action);
        GoldRushAction* setLast(GoldRushAction *action);
        void printName();
        virtual GoldRushAction* run(Robot* robot);
    protected:
        GoldRushAction *nextAction;
        std::string name = "Gold Rush Action (NAME NOT CHANGED)";
};

#endif