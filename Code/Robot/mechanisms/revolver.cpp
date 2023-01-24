#include "revolver.h"
#include <memory>

Revolver::Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h)
{
    enc = h;

    //TODO: Update Pins
    motor_revolver = std::make_shared<Motor>(17, 0, pca);
    agitator = std::make_shared<Motor>(17, 0, pca);

    loader = std::make_shared<Servo>(17, pca);
    opener = std::make_shared<Servo>(17, pca);
    dropper = std::make_shared<Servo>(17, pca);

    //Initializing Revolver and Can to Empty
    revolver = [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY];
    can = [EMPTY, EMPTY, EMPTY]; // 0 is the top position (the first loaded)
    //TODO Find encoder ticks for each position
    chambers = [0, 1, 2, 3, 4];

    opener = false;
    dropper = false;
    finger_in_revolver = false;
}

// Look at the name
Revolver::turn_on_agitator()
{
    Revolver::agitator->setPower(1.0);
}

// Look at the name
Revolver::turn_off_agitator()
{
    Revolver::agitator->setPower(0.0);
}

/*  Rotates the revolver to one of the chambers (int pos)
    TODO Edit how the rotation logic works
    @param int pos (location in encoder ticks of where it needs to go)
*/
Revolver::rotate_revolver(int pos)
{ 
    //TODO Find Suitable Power to run the motor at and which direction it should turn
    double power = (encoder_ticks - chambers[pos] < 0) ? 1.0 : -1.0;

    
    while (get_current_position() != pos)
    {
        motor_revolver->setPower(power);
    }

    motor_revolver->setPower(0.0);
}

// Toggles the servo in control of dropping/storing the can
Revolver::toggle_drop_servo()
{
    if (dropper)
    {
        dropper->setPosition(0, 270);
        dropper = false;
    }
    else
    {
        dropper->setPosition(270, 270);
        dropper = true;
    }
}

// Toggles the servo in control of opening/closing the can
// Resets can and check_can if it opens
Revolver::toggle_open_servo()
{
    if (opener)
    {
        opener->setPosition(0, 270);
        opener = false;
    }
    else
    {
        opener->setPosition(270, 270);
        opener = true;
        for (int i = 0; i < 3; i++)
        {
            can[i] = EMPTY;
        }
        check_can = 3;
    }
}


Revolver::store_marshmallow(MARSHMALLOWS color)
{
    int goal_chamber = get_color_pos(EMPTY);
    if (goal_chamber == -1)
        return -1;
    
    rotate_revolver(chambers[goal_chamber] + agitator_mod);
    return 1;
}

Revolver::get_color_pos(MARSHMALLOWS color)
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

Revolver::retract_loader()
{
    loader->setPosition(0, 270);
    finger_in_revolver = false;
}

Revolver::get_finger_in_revolver()
{
    return finger_in_revolver;
}

Revolver::load_marshmallow(MARSHMALLOWS color)
{
    int chamber = get_color_pos(color);
    // The color wasn't loaded in the revolver
    if (chamber == -1)
        return -1;
    
    rotate_revolver(chamber);
    loader->setPosition(270, 270); // Again need to adjust servo values
    finger_in_revolver = true;
    revolver[chamber] = EMPTY;


    //TODO find a better way to do this part (probably w/ check_can)
    int count = 0; 
    while (can[count] != EMPTY)
    {
        count++;
    }
    can[count] = color;

    return count;
}

// Three tall Statue: base level – white, second level – green, third level – red
// with a pink duck on top in the middle of the pond
// Two Tall Statue: base level – white, second level – green
// with a yellow duck on top on the outside pond locations