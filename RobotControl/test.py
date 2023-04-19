from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors
from Robot.Marshmallows import PringleStates
from Actions.marshmallowActions import LoadStack
from Actions.foodActions import FoodAction 
from Actions.StartupActions import StartupActions
from main import runEachLoop
from constants import *


robot = Robot()


startup = StartupActions()
food = FoodAction()

robot.marshmallow.set_loader(False)
robot.marshmallow.revolver.run(0)
robot.drivetrain.driveM(0, 0, 0, 0)
robot.marshmallow.pringle_can.runPWM(0)
robot.marshmallow.placer.runPWM(0)


#robot.marshmallow.revolver_enc
a = input("r to reset")
print(a)

if a.strip() == "r":
    robot.marshmallow.revolver_enc.reset()
    robot.drivetrain.position = [START_X, START_Y, 0]
    print("Reset")

while True:
    runEachLoop(robot)
    if not food.run(robot):
        break
   
"""


robot.intake.intake_marshmallow()


input()


robot.intake.stopIntaking()

"""