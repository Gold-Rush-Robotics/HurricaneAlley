#include "Runner.h"

class Auto : Runner
{
    int init() override
    {

    }
    int loop() override
    {

    }
    int initLoop() override {

    }
};

int main(int argc, char const *argv[])
{
    Auto::init();
    return 0;
}
