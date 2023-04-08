from GoldRushAction import GoldRushAction
from Robot.Robot import Robot

from constants import *

class FoodAction(GoldRushAction):
    state: int = 0
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                ## Intake init
                state += 1 
            case 1:
                robot.drivetrain.driveToPoint([FOOD2[0] + FOOD2[2] + 15, FOOD2[1] + (FOOD2[3]/2), PI/2])
            
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