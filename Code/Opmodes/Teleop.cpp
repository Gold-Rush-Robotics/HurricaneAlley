#include "Runner.h"

class Teleop : Runner
{
    int init() override
    {
    }
    int loop() override
    {
    }
};

int main(int argc, char const *argv[])
{
    Teleop::main();
    return 0;
}
