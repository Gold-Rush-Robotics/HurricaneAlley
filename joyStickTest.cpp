/**
 * Author: Jason White
 *
 * Description:
 * Reads joystick/gamepad events and displays them.
 *
 * Compile:
 * gcc joystick.c -o joystick
 *
 * Run:
 * ./joystick [/dev/input/jsX]
 *
 * See also:
 * https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

#define NORM_SPEED 0.5
#define SLOW_SPEED 0.3
#define FAST_SPEED 1.0
#define AXIS_MAX 32767.0

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

struct mecanumVector {
    double x, y, h;
};

struct buttonStore {
    bool x, c, t, s, rBump, lBump;
};

int buttonHandler(buttonStore butH, struct js_event *event) {
    if(event->number == 4) {
        butH.lBump = event->value;
        printf(event->value ? "4 pressed" : "4 released");
    }
    
}

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[4])
{
    size_t axis = event->number;

    //printf("axis->number: %d\n", axis);

    if (axis == 0) {
        axes[0].x = event->value;
        return 0;
    } else if (axis == 1) {
        axes[0].y = event->value;
        return 0;
    } else if (axis == 2) {
        axes[1].x = event->value;
        return 1;
    } else if (axis == 5) {
        axes[1].y = event->value;
        return 1;
    } else if (axis == 3) {
        axes[2].x = event->value;
        return 2;
    } else if (axis == 4) {
        axes[2].y = event->value;
        return 2;
    } else if (axis == 6) {
        axes[3].x = event->value;
        return 3;
    } else if (axis == 7) {
        axes[3].y = event->value;
        return 3;
    }

/*
    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }
*/
    return  -1;
}

int main(int argc, char *argv[])
{
    const char *device;
    int js;
    struct js_event event;
    struct axis_state axes[4] = {0};
    struct mecanumVector mVec;
    struct buttonStore but;
    size_t axis;

    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                buttonHandler(but, &event);
                printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                break;
            case JS_EVENT_AXIS:
                axis = get_axis_state(&event, axes);
                /*if (axis < 4)
                    printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
                break;*/
            default:
                /* Ignore init events. */
                break;
        }
        
        fflush(stdout);
        
        printf(but.lBump ? "true\n" : "false\n");

        if(axes[2].x > 0) {
            //Slow Speed
            mVec.x = axes[0].x * SLOW_SPEED / AXIS_MAX;
            mVec.y = axes[0].y * -SLOW_SPEED / AXIS_MAX;
            mVec.h = axes[1].x * -SLOW_SPEED / AXIS_MAX;
        } else if (but.lBump) {
            //Fast Speed
            mVec.x = axes[0].x * FAST_SPEED / AXIS_MAX;
            mVec.y = axes[0].y * -FAST_SPEED / AXIS_MAX;
            mVec.h = axes[1].x * -FAST_SPEED / AXIS_MAX;
        } else {
            //Normal Speed
            mVec.x = axes[0].x * NORM_SPEED / AXIS_MAX;
            mVec.y = axes[0].y * -NORM_SPEED / AXIS_MAX;
            mVec.h = axes[1].x * -NORM_SPEED / AXIS_MAX;
        }
        

        printf("x: %.2f y: %.2f h: %.2f\n", mVec.x, mVec.y, mVec.h);
    }

    close(js);
    return 0;
}