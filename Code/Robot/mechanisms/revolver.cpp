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

Revolver::get_current_chamber()
{
    //TODO Find the encoder number for revolver
    encoder_ticks = enc->getPos(3);

    //TODO add some kind of range that each chamber is inbetween
    if (encoder_ticks == chambers[0])
        return 0;
    if (encoder_ticks == chambers[1])
        return 1;
    if (encoder_ticks == chambers[2])
        return 2;
    if (encoder_ticks == chambers[3])
        return 3;
    if (encoder_ticks == chambers[4])
        return 4;
}

// Rotates the revolver to one of the chambers (int pos)
// TODO Edit how the rotation logic works
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

// TODO for release and store can add some kind of delay to make sure that servo did the thing
Revolver::release_can()
{
    //TODO Find Position for Servo and max Angle
    dropper->setPosition(0, 270);
    opener->setPosition(0, 270);

    for (int i = 0; i < 3; i++)
    {
        can[i] = EMPTY;
    }
    check_can = 3;
}

Revolver::store_can()
{
    //TODO Find Position for Servo and max Angle
    dropper->setPosition(270, 270);
    opener->setPosition(270, 270);
}

// TODO Make it not rotate one loop to findout that there's an available chamber
//      ( contains_color(empty) )
Revolver::store_marshmallow(MARSHMALLOWS color)
{
    int initial_chamber = 5;
    int current_chamber = get_current_chamber();
    //TODO find the modifier of ticks between chambers from the loader to the agitator
    //     Maybe find a more optimal way of storing marshmallows other than first come first serve
    while (revolver[current_chamber] != EMPTY && initial_chamber != current_chamber)
    {
        if (initial_chamber == 5)
            initial_chamber = current_chamber;
        
        if (current_chamber + 1 > 4)
            current_chamber = 0;
        
        rotate_revolver(current_chamber + 1);
        current_chamber = get_current_chamber();
    }
    
    if (initial_chamber == current_chamber)
    {
        // This means that there wasn't an open slot
        return -1;
    }
    else 
    {
        revolver[current_chamber] = color;
        return 1;
    }
}

Revolver::contains_color(MARSHMALLOWS color)
{
    for (int i = 0; i < 5; i++)
    {
        if (revolver[i] == color)
            return i;
    }
    return -1;
}

Revolver::load_marshmallow(MARSHMALLOWS color)
{
    int chamber = contains_color(color);
    // The color wasn't loaded in the revolver
    if (chamber == -1)
        return -1;
    
    rotate_revolver(chamber);
    loader->setPosition(270, 270); // Again need to adjust servo values
    loader->setPosition(0, 270);
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

//TODO maybe have a method to store one at a time instead of waiting to have both a white and a green at least
//     also maybe have optimized storage
// Three tall Statue: base level – white, second level – green, third level – red
// with a pink duck on top in the middle of the pond
// Two Tall Statue: base level – white, second level – green
// with a yellow duck on top on the outside pond locations
Revolver::load_marshmallow_stack(int stack_size)
{
    if (check_can != 3)
        return -1;

    if (stack_size == 2)
    {
        // Load the green first        
        check_can = load_marshmallow(GREEN);
        if (check_can == -1)
            return -2;
        // Load the white second
        check_can = load_marshmallow(WHITE);
        if (check_can == -1)
            return -3;

        return 2;
    }

    if (stack_size == 3)
    {
        // Load the red first
        check_can = load_marshmallow(RED);
        if (check_can == -1)
            return -4;
        
        // Load the green second
        check_can = load_marshmallow(GREEN);
        if (check_can == -1)
            return -5;
        
        // Load the white third
        check_can = load_marshmallow(WHITE);
        if (check_can == -1)
            return -6;

        return 3;
    }

    // Nothing got loaded because of an invalid stack size
    return 0;
}