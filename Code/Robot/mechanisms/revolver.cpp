#include "revolver.h"
#include <memory>
#include <iostream>

int REV_TOLERANCE = 5;
int stable = 0;

Revolver::Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h)
{
    enc = h;

    //TODO: Update Pins
    motor_revolver = std::make_shared<Motor>(27, 7, pca);
    agitator = std::make_shared<Motor>(17, 6, pca); // 17 6 is for real agitator current values are duck
    
    loader = std::make_shared<Servo>(10, pca);
    opener = std::make_shared<Servo>(5, pca);
    dropper = std::make_shared<Servo>(4, pca);

    //TODO Find encoder ticks for each position
    is_open = false;
    dropper_up = false;
    finger_in_revolver = false;

    // Add colorsensor to constructor
    colorsensor = new ColorSensor();

    revolverPID = new PID(0.1, 1.0, -1.0, 0.01, 0.0, 0.0);
}

// Look at the name
void Revolver::turn_on_agitator()
{
    Revolver::agitator->setPower(.5); // .5
}

// Look at the name
void Revolver::turn_off_agitator()
{
    Revolver::agitator->setPower(0.0);
}

/*  Rotates the revolver to encoder value (int pos)
    @param int pos (location in encoder ticks of where it needs to go)
*/
bool Revolver::rotate_revolver(double pos)
{ 

    int current = enc->getPos(3);
    std::cout << "Goal: " << pos << " | Current: " << current << std::endl;
    double speed = revolverPID->calculate(pos, current);

    
    if(std::abs(current-pos) <= REV_TOLERANCE){
        motor_revolver->setPower(0);
        stable++;
        if(stable >=10){
            return true;
        }
    } else {
        stable = 0;
        motor_revolver->setPower(speed);
    }
    return false;
}

void Revolver::rotate_speed(double speed){
    motor_revolver->setPower(speed);
}

// Toggles the servo in control of dropping/storing the can
void Revolver::drop_servo(bool down)
{
    
    if (down)
    {
        dropper->setPosition(1800, 300);
    }
    else
    {
        dropper->setPosition(3750, 300);
    }
}

// Toggles the servo in control of opening/closing the can
// Resets can and check_can if it opens
void Revolver::pringle_servo(PRINGLE_STATES state)
{
    switch(state){
        case PRINGLE_STATES::CLOSED:
            opener->setPosition(1900, 300);
            break;
        case PRINGLE_STATES::ACCEPTING:
            opener->setPosition(2075, 300);
            break;
        case PRINGLE_STATES::OPEN:
            opener->setPosition(2500, 300);
            break;
    }
}


bool Revolver::store_marshmallow()
{
    int goal_chamber = get_color_pos(EMPTY);
    if (goal_chamber == -1)
        return -1;
    if(rotate_revolver(chambers[goal_chamber] + agitator_mod)){
        revolver[goal_chamber] = get_color();
        return true;
    }
    return false;
}

int Revolver::get_color_pos(MARSHMALLOWS color)
{
    for (int i = 0; i < 5; i++)
    {
        if (revolver[i] == color)
            return i;
    }
    return -1;
}

// There might be a problem with load_marshmallow where the finger used to load the can
// will need to retract before the next time rotate_revolver is called (which should
// be in load_marshmallow (or possibly store_marshmallow) but the finger might not 
// have enough time to get out of the way of the chamber revolving which is why I had 
// to get rid of load_marshmallow_stack ;(

void Revolver::retract_loader()
{
    loader->setPosition(800, 270);
}

void Revolver::insert_loader(){
    loader->setPosition(3800, 270);
}

bool Revolver::get_finger_in_revolver()
{
    return finger_in_revolver;
}

bool Revolver::load_marshmallow(MARSHMALLOWS color)
{
    int chamber = get_color_pos(color);
    // The color wasn't loaded in the revolver
    if (chamber == -1)
        return false;

    if(rotate_revolver(chambers[chamber])){
        revolver[chamber] = MARSHMALLOWS::EMPTY;
        return true;
    }

    return false;
}

MARSHMALLOWS Revolver::get_color()
{
    return colorsensor->get_color();
}

// Three tall Statue: base level – white, second level – green, third level – red
// with a pink duck on top in the middle of the pond
// Two Tall Statue: base level – white, second level – green
// with a yellow duck on top on the outside pond locations