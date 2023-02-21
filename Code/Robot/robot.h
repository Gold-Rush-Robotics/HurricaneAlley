#include <iostream>
#include "bcm2835.h"
#include "pca9685.h"
#include "drivetrain.h"
#include "mechanisms/revolver.h"

class Robot{
    public:
        std::shared_ptr<PCA9685> pca9685;
        std::shared_ptr<Revolver> revolver;
        void start();
        void driveController(double s1y, double s1x, double s2y, double s2x, double multiplier);
        void stop();
        void reset();
        void printEncoders();
    private:
        std::shared_ptr<Drivetrain> drivetrain;
        std::shared_ptr<EncoderHandler> encoderHandler;
        
};