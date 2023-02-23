#include "../Robot/robot.h"

#include <linux/joystick.h>
#include "ps5Controller.cpp"
#include <iostream>

// Reads controller event and insures it is of correct type
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

void sleep_ms(uint ms)
{
    struct timespec req;
    req.tv_nsec = ms * 1000 * 1000;
    req.tv_sec = 0;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
}
int main(int argc, char *argv[])
{
    if (getuid() != 0)
    {
        fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
        return -1;
    }
    Robot robot;
    robot.start();
    const char *device;
    int js;
    struct js_event event;
    ps5Controller ctr = ps5Controller();
    pthread_t odoTID;
    pthread_t printTID;
    double multiplier = .7;
    double s1x = 0.0;
    double s2x = 0.0;
    double s1y = 0.0;
    double s2y = 0.0;
    
        if (argc > 1)
            device = argv[1];
        else
            device = "/dev/input/js0";

        js = open(device, O_RDONLY);

        if (js == -1)
            perror("Could not open joystick");
    //robot.printEncodersLoop();

    while(true){
        robot.printEncoders();
    }
    robot.stop();
}
