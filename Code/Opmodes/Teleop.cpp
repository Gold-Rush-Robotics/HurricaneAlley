#include "Runner.h"

class Teleop : Runner
{
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
    int init() override
    {
        robot.start();
        loop();
    }
    int loop() override
    {
        const char *device;
        int js;
        struct js_event event;
        ps5Controller ctr = ps5Controller();
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
        while (read_event(js, &event) == 0)
        {
            ctr.eventHandler(&event);
            // ctr.printStates();
            double multiplier = .75;
            if (ctr.lBump)
            {
                multiplier = 1;
            }
            else if (ctr.lTrigBool)
            {
                multiplier = .5;
            }
            robot.driveController(ctr.ly, ctr.lx, ctr.ry, ctr.rx, multiplier);
            fflush(stdout);
        }
        robot.stop();
    }
};

int main(int argc, char const *argv[])
{
    Teleop::init();
    return 0;
}
