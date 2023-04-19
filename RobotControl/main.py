from Actions.runActions import getRun, runEachLoop
from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from constants import *


if __name__ == "__main__":
    robot = Robot()
    robot.marshmallow.revolver.run(0)
    robot.drivetrain.driveM(0, 0, 0, 0)
    robot.marshmallow.revolver_enc.reset()
    robot.drivetrain.position = [START_X, START_Y, 0]

    actionTree: GoldRushAction = getRun()
    
    robot.drivetrain.drivePow(0, 0, 0)
    
    while(actionTree):
        runEachLoop(robot)
        print(actionTree.description)
        actionTree = actionTree.run(robot)
