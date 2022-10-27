#include "Robot/robot.h"

#include <linux/joystick.h>
#include "ps5Controller.cpp"
#include <iostream>

int main(int argc, char *argv[]){
    if (getuid() != 0)
    {
        fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
        return -1;
    }
    Robot* robot;
    robot->start();
    const char *device;
    int js;
    struct js_event event;
    ps5Controller ctr = ps5Controller();
    pthread_t odoTID;
    pthread_t printTID;
    double multiplier = .7;

    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    while (read_event(js, &event) == 0) {
        ctr.eventHandler(&event);

        // ctr.printStates();

        double multiplier = .75;
        if (ctr.lBump) {
            multiplier = 1;
        }
        else if (ctr.lTrigBool) {
            multiplier = .5;
        }

        robot->driveController(ctr.ly, ctr.lx, ctr.ry,ctr.rx, multiplier);

        fflush(stdout);
    }

    robot->stop();
}




/**
int main(int argc, char *argv[]){
    std::cout << "spot 0";
    if (getuid() != 0)
    {
        fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
        return -1;
    }
    std::cout << "spot 1";
    Robot robot = Robot();
    const char *device;
    std::cout << "spot 2";
    int js;
    struct js_event event;
    ps5Controller ctr = ps5Controller();
    std::cout << "spot 3";
    pthread_t odoTID;
    pthread_t printTID;
    double multiplier = .7;

    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    std::cout << "spot 4";
    js = open(device, O_RDONLY);
    std::cout << "spot 5";

    if (js == -1)
        perror("Could not open joystick");

    // This loop will exit if the controller is unplugged. 
    while (read_event(js, &event) == 0) {
        ctr.eventHandler(&event);

        // ctr.printStates();

        double multiplier = .75;
        if (ctr.lBump) {
            multiplier = 1;
        }
        else if (ctr.lTrigBool) {
            multiplier = .5;
        }

        robot.driveController(ctr.ly, ctr.lx, ctr.ry,ctr.rx, multiplier);

        fflush(stdout);
    }

    robot.stop();
}
*/
