#include "../Robot/robot.h"
#include "../Actions/GoldRushAction.h"
#include <pthread.h>

/*base runner class to be extended by teleop and auto*/
class Runner{
    public:
        int seconds = 0;
        Robot robot;
        int init(int argc, char *argv[]);
        int loop(int argc, char *argv[]);
        int initLoop(int argc, char *argv[]);
        int spawnThreads();
        pthread_t encoder;
        pthread_t print;
        
    protected:
        GoldRushAction* currentAction;
};