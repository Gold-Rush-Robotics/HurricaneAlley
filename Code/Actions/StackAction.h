#include "GoldRushAction.h"

#ifndef STACKACTIONH
#define STACKACTIONH

class StackAction : public GoldRushAction {
    public:
        enum StackHeight {
            TWO = 2,
            THREE = 3
        };
        StackAction(StackHeight height);
        GoldRushAction* run(Robot* robot);
    private:
        StackHeight height;
};

#endif