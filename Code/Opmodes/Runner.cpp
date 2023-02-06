#include "Runner.h"

/* This function initializes all the needed values
Base Behavior: initialize robot and advance to the initLoop*/
int Runner::init(int argc, char *argv[]){
    //Initialize all the thingies
    Runner::robot.start();
    Runner::initLoop(argc, argv);
    return 0;
}

/* This function will run after init until told to go to loop (light turning on)
Base behavior: Imediatly advance*/
int Runner::initLoop(int argc, char *argv[]){
    return Runner::loop(argc, argv);
}
/* Loops Forever, and handles the processes
Base behavior: Imediately die*/
int Runner::loop(int argc, char *argv[]){
    return 0;
}

int Runner::spawnThreads(){
    return 0;
}
