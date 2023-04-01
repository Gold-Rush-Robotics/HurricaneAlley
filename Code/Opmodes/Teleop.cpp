#include "Runner.h"
#include "../Robot/mechanisms/revolver.h"

#include <linux/joystick.h>
#include "ps5Controller.cpp"
#include <time.h>

#include "../Actions/StoreMarshmellow.h"
#include "../Actions/StackAction.h"
#include "../Actions/placeAction.h"


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
                    robot.revolver->drop_servo(true);
                } else {
                    robot.revolver->drop_servo(false);
                }
                if (ctr.s)
                {
                    robot.revolver->pringle_servo(Revolver::PRINGLE_STATES::OPEN);
                } else {
                    robot.revolver->pringle_servo(Revolver::PRINGLE_STATES::ACCEPTING);
                }

                if (ctr.c)
                {
                    robot.revolver->rotate_speed(1);
                } else if (ctr.lBump){
                    robot.revolver->rotate_speed(-1);
                } else {
                    robot.revolver->rotate_speed(0);
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

        void runAutoActions(){
            Robot* robotPtr = &robot;
            while(currentAction != nullptr){
                        currentAction->printName();
                        robot.drivetrain->encoderLogic();
                        currentAction = currentAction->run(robotPtr);
            }
            std::cout << "Re-entering Teleop Mode" << std::endl;
        }

        void marshmallowControls(js_event event, ps5Controller ctr, int js){
            std::cout << "Marshmallow Controls Mode:" << std::endl;
            double multiplier = .7;
            StackAction::StackHeight currentStack = StackAction::StackHeight::THREE;
            while (read_event(js, &event) == 0)
            {
                ctr.eventHandler(&event);

                if(ctr.x){
                    //white marshmellow
                    
                    currentAction = new StoreMarshmellow(MARSHMALLOWS::WHITE);
                    runAutoActions();
                    
                }
                if(ctr.s){
                    //add a green marshmallow
                    currentAction = new StoreMarshmellow(MARSHMALLOWS::GREEN);
                    runAutoActions();
                }
                if(ctr.c){
                    //add a red marshmallow
                    currentAction = new StoreMarshmellow(MARSHMALLOWS::RED);
                    runAutoActions();
                }

                if(ctr.t){
                    currentAction = new StackAction(StackAction::StackHeight::THREE);
                    currentStack = StackAction::StackHeight::THREE;
                    runAutoActions();
                }

                if(ctr.rBump){
                    currentAction = new StackAction(StackAction::StackHeight::TWO);
                    currentStack = StackAction::StackHeight::TWO;
                    runAutoActions();
                }
                if(ctr.lBump){
                    currentAction = new PlaceAction(currentStack);
                    runAutoActions();
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
