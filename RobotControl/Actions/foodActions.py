from GoldRushAction import GoldRushAction
from marshmallowActions import LoadStack
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors
from placementActions import PlaceStack

from constants import *

class FoodAction(GoldRushAction):
    state: int = 0
    food2Red: bool = False
    loadStack: LoadStack
    placeStack: PlaceStack
    flag: False
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.loadStack = LoadStack("Load food chip")
        self.placeStack = PlaceStack("Place food Chip")

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                ## Camera Release
                robot.intake.dm_separator_servo.run(DM_SEPARATOR_INIT_CAMERA)
                self.state += 1
            case 1:
                if robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2] + 15, FOOD2[1] + (FOOD2[3]/2), PI/2], 5, 0.01):
                    state += 1
            case 2:
                self.food2Red = robot.vision.detectRedFood()
            case 3:
                load = self.loadStack.run(robot, (MarshmallowColors.REDFOOD) if self.food2Red else (MarshmallowColors.GREENFOOD), self.flag)
                self.flag = not load
                drive = robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2], FOOD2[1], -PI/4])
                if(load and drive):
                    self.flag = False
                    state+=1
            case 4:
                if not self.placeStack.run(robot, False):
                    state += 1
            case 5:
                load = self.loadStack.run(robot, (MarshmallowColors.GREENFOOD) if self.food2Red else (MarshmallowColors.REDFOOD), self.flag)
                self.flag = not load
                drive = robot.drivetrain.driveToPoint([FOOD1[0] + FOOD1[2], FOOD1[1] + FOOD1[3], -3*PI/4])
                if(load and drive):
                    self.flag = False
                    state+=1
            case 6:
                if not self.placeStack.run(robot, False):
                    return self.nextAction
        return self