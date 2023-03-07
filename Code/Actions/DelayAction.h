#include "GoldRushAction.h"
#include <chrono>

#ifndef DELAYACTIONH
#define DELAYACTIONH

class DelayAction : public GoldRushAction {
    public:
        DelayAction(double seconds);
        GoldRushAction* run(Robot* robot);
    private:
        std::chrono::steady_clock::time_point begin;
        bool flag = false;
        double seconds;

};

#endif