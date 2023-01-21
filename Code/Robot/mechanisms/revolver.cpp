#include "revolver.h"
#include <memory>

Revolver::Revolver(std::shared_ptr<PCA9685> pca){
    pca9685 = pca;
    //TODO: Update Pins
    revolver = std::make_shared<Motor>(17, 0, pca);
    agitator = std::make_shared<Motor>(17, 0, pca);

    loader = std::make_shared<Servo>(17, pca);
    opener = std::make_shared<Servo>(17, pca);
    dropper = std::make_shared<Servo>(17, pca);
}

Revolver::turn_on_agitator(){
    Revolver::agitator->setPower(1.0);
}

Revolver::turn_off_agitator(){
    Revolver::agitator->setPower(0.0);
}

Revolver::rotate_revolver(){

}

Revolver::release_can(){

}

Revolver::store_can(){

}

Revolver::rotate_revolver(){

}

Revolver::load_marshmallow(){

}