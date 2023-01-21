#include "revolver.h"
#include <memory>

Revolver::Revolver(std::shared_ptr<PCA9685> pca, std::shared_ptr<EncoderHandler> h)
{
    pca9685 = pca;
    enc = h;
    
    // Initializing Encoder stuff here until phillip says it's wrong
    enc->init();

    //TODO: Update Pins
    motor_revolver = std::make_shared<Motor>(17, 0, pca);
    agitator = std::make_shared<Motor>(17, 0, pca);

    loader = std::make_shared<Servo>(17, pca);
    opener = std::make_shared<Servo>(17, pca);
    dropper = std::make_shared<Servo>(17, pca);

    //Initializing Revolver to Empty
    revolver = [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY];
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

// Rotates the revolver to one of the chambers (int pos)
Revolver::rotate_revolver(int pos)
{
    //TODO Find the encoder number for revolver
    encoder_ticks = enc->getPos(3);

    //TODO Find Suitable Power to run the motor at and which direction it should turn
    double power = (encoder_ticks - chambers[pos] < 0) ? 1.0 : -1.0;

    //TODO add some kind of range that it can be between
    while (enc->getPos(3) != chambers[pos])
    {
        motor_revolver->setPower(power);
    }
}

Revolver::release_can()
{

}

Revolver::store_can()
{

}

Revolver::load_marshmallow()
{

}