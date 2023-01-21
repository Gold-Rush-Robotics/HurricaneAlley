#include "../Robot/robot.h"
#include "../Actions/GoldRushAction.h"

class Runner{
    public:
        int seconds = 0;
        Robot robot;
        virtual int init();
        virtual int loop();
    private:
        GoldRushAction currentAction;
};