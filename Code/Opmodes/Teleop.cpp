#include "Runner.h"

#include <linux/joystick.h>
#include "ps5Controller.cpp"
#include <time.h>

class Teleop : Runner
{
    public:
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
        int init(int argc, char *argv[])
        {
            robot.start();
            spawnThreads();

            loop(argc, argv);
            return 0;
        }
        void standardControls(js_event event, ps5Controller ctr, int js){
            std::cout << "Standard Controls Mode:" << std::endl;
            double multiplier = .7;
            while (read_event(js, &event) == 0)
            {
                ctr.eventHandler(&event);
                robot.driveController(ctr.ly, ctr.lx, ctr.ry, ctr.rx, multiplier);
                if (ctr.x)
                {
                    robot.revolver->turn_on_agitator();
                } else {
                    robot.revolver->turn_off_agitator();
                }
                if (ctr.t)
                {
                    robot.revolver->toggle_drop_servo();
                }
                else if (ctr.s)
                {
                    robot.revolver->toggle_open_servo();
                }
                else if (ctr.c)
                {
                    robot.revolver->rotate_revolver(1);
                } else if (ctr.lBump){
                    robot.revolver->rotate_revolver(-1);
                } else {
                    robot.revolver->rotate_revolver(0);
                }
                if (ctr.rBump) {
                    robot.revolver->insert_loader();
                } else {
                    robot.revolver->retract_loader();
                }
                

                robot.driveController(ctr.ly, ctr.lx, ctr.ry, ctr.rx, multiplier);
                robot.drivetrain->printPosition();
                robot.drivetrain->encoderLogic();
                fflush(stdout);
            }
            robot.stop();
        }

        void marshmallowControls(js_event event, ps5Controller ctr, int js){
            std::cout << "Marshmallow Controls Mode:" << std::endl;
            double multiplier = .7;
            while (read_event(js, &event) == 0)
            {
                ctr.eventHandler(&event);

                if(ctr.x){
                    //add a white marshmallow
                }
                if(ctr.s){
                    //add a green marshmallow
                }
                if(ctr.c){
                    //add a red marshmallow
                }

                if(ctr.t){
                    //build a three stack
                }

                if(ctr.rBump){
                    //build a two stack
                }
            }
        }

        int loop(int argc, char *argv[])
        {
            const char *device;
            int js;
            struct js_event event;
            ps5Controller ctr = ps5Controller();
            if (argc > 1)
                device = argv[1];
            else
                device = "/dev/input/js0";
            js = open(device, O_RDONLY);
            if (js == -1)
                perror("Could not open joystick");

            bool toggle = false;

            std::cout << "TELEOP MODE SELECT: \nX:Marshmallow Logic Testing \nC:Standard Mode" << std::endl;
            
            while(read_event(js, &event) == 0){
                ctr.eventHandler(&event);
                if(ctr.x){
                    toggle = true;
                    break;
                }
                if(ctr.c){
                    break;
                }
            }
            if(toggle){
                marshmallowControls(event, ctr, js);
            } else {
                standardControls(event, ctr, js);
            }

            


            
            return 0;
        }
};

int main(int argc, char *argv[])
{
    std::cout << "HELLO " << std::endl;
    fflush(stdout);
    Teleop t;
    t.init(argc, argv);
    return 0;
}
