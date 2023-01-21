#include "../Robot/robot.h"
#include "../Actions/GoldRushAction.h"

/*base runner class to be extended by teleop and auto*/
class Runner{
    public:
        int seconds = 0;
        Robot robot;
        virtual int init();
        virtual int loop();
        virtual int initLoop();
    private:
        GoldRushAction currentAction;
};