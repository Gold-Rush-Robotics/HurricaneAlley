from Actions.GoldRushAction import GoldRushAction
from Actions.marshmallowActions import LoadStack
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors
from Actions.placementActions import PlaceStack
from Hardware.actuators import PWMMotor
from adafruit_pca9685 import PCA9685
from microcontroller import Pin
from constants import *

import time

class FoodAction(GoldRushAction):
    state: int = 0
    food2Red: bool = False
    loadStack: LoadStack
    placeStack: PlaceStack
    flag: False
    dir : Pin = Pin(27)
    
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.loadStack = LoadStack("Load food chip")
        self.placeStack = PlaceStack("Place food Chip")
        self.flag = False

    def run(self, robot: Robot) -> GoldRushAction:
        print(self.state)
        match(self.state):
            case 0:

                load = self.loadStack.run(robot, [MarshmallowColors.CLOSE], self.flag)
                if not load:
                    self.state += 1

            case 1:
                time.sleep(2)
                robot.drivetrain.drivePow(-0.1, -0.4, 0)
                time.sleep(5)
                robot.drivetrain.drivePow(0.3,0,0)
                time.sleep(1)
                robot.drivetrain.drivePow(0,0,0)
                self.state = 2
            case 2:
                # if(robot.drivetrain.driveToPoint([30, 85, 0], 2, 0.1)):
                #     self.state += 1
                self.state = 3
            case 3:
                if not self.placeStack.run(robot, False):
                    self.state += 1
                    self.placeStack.state = 0
            case 4:
                robot.drivetrain.drivePow(0, 0.2, 0)
                time.sleep(.15)
                robot.drivetrain.drivePow(0,0,0.5)
                time.sleep(1.25)
                robot.drivetrain.drivePow(0,0,0)
                time.sleep(2)
                robot.drivetrain.drivePow(-0.6, 0.4, 0)
                time.sleep(5)
                robot.drivetrain.drivePow(0.2, 0, 0)
                time.sleep(1.5)
                robot.drivetrain.drivePow(0,0,0)
                time.sleep(1)
                self.state += 1
                self.loadStack.state = 0
            case 5:
                #if self.loadStack.run(robot, (MarshmallowColors.FAR), False):
                #    self.state += 1
                self.state = 7
            case 6:
                #if not self.placeStack.run(robot, False):
                #    self.state += 1
                pass
            case 7:
                time.sleep(1)
                robot.drivetrain.drivePow(0,-0.5,0)
                time.sleep(5)
                robot.drivetrain.drivePow(0,0.5,0)
                time.sleep(3)
                robot.drivetrain.drivePow(-0.2,0,0)
                time.sleep(1)
                robot.drivetrain.drivePow(-0.1, -0.5, 0)
                time.sleep(5)
                robot.drivetrain.drivePow(0,0,0)
                time.sleep(1)
                robot.drivetrain.drivePow(0.2)
                




                # load = self.loadStack.run(robot, (MarshmallowColors.FAR), self.flag)
                # self.flag = not load
                # drive = robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2], FOOD2[1], 3*PI/4], 5, 0.1)
                # if(load and drive):
                #     self.flag = False
                #     self.state+=1
                # robot.marshmallow.place_pringle(False)
                # time.sleep(2)
                # robot.drivetrain.drivePow(0, 0, 0)
                # robot.clear()
            case 7:
                if not self.placeStack.run(robot, False):
                    input()
                    return self.nextAction
        return self