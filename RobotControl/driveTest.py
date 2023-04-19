from Robot.Robot import Robot
from Actions.runActions import runEachLoop
from constants import *

robot = Robot()

robot.drivetrain.encoderHandler.reset()
robot.drivetrain.position = [0, 0, 0]

robot.drivetrain.driveM(0, 0, 0, 0)

input()



while True:
    runEachLoop(robot)
    #rint(robot.drivetrain.position)
    if robot.drivetrain.driveToPoint([-100, 0, 0], .5, .01):
        pass