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
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.loadStack = LoadStack("Load food chip")
        self.placeStack = PlaceStack("Place ")

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                ## Intake init
                state += 1 
            case 1:
                if robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2] + 15, FOOD2[1] + (FOOD2[3]/2), PI/2], 5, 0.01):
                    state += 1
            case 2:
                self.food2Red = robot.vision.detectRedFood()
            case 3:
                load = self.loadStack.run(robot, (MarshmallowColors.REDFOOD) if self.food2Red else (MarshmallowColors.GREENFOOD))
                drive = robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2], FOOD2[1], -PI/4])
                if(load and drive):
                    state+=1
            case 4:
                pass
        """
         init intake
         drive to spot2
         visionnnn
         go to the top-Right of food2 (45 degrees)
         Load the right food chip
         Place it
         go to the bottom right of food1 (135 degrees)
         load the next food chip
         place it

         if(red):
            
         if(green):
        

        """
        robot.drivetrain.position