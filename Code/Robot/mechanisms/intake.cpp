#include "intake.h"
#include <memory>
#include <iostream>


Intake::Intake(std::shared_ptr<PCA9685> pca)
{
    //TODO: Update Pins
    intake_motor = std::make_shared<Motor>(27, 7, pca);
    
    splitter = std::make_shared<Servo>(10, pca);

}

void Intake::turn_on_intake()
{
    intake_motor->setPower(0.5);
}

void Intake::turn_off_intake()
{
    intake_motor->setPower(0.0);
}

// TODO: Tune Servo Values
void Intake::intake_marshmallow()
{
    splitter->setPosition(123, 0);
}

void Intake::intake_duck_agitator()
{
    splitter->setPosition(123, 0);
}

void Intake::intake_duck_bag()
{
    splitter->setPosition(123, 0);
}