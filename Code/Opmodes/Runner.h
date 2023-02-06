#include "../Robot/robot.h"
#include "../Actions/GoldRushAction.h"

/*base runner class to be extended by teleop and auto*/
class Runner{
    public:
        int seconds = 0;
        Robot robot;
        int init(int argc, char *argv[]);
        int loop(int argc, char *argv[]);
        int initLoop(int argc, char *argv[]);
        int spawnThreads();
    protected:
        GoldRushAction* currentAction;
};