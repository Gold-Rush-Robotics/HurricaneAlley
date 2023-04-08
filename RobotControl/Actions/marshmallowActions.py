from __future__ import annotations
from GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors, PringleStates
from DelayAction import DelayAction
import numpy as np
PRINGLE_DELAY = 0.5
LOADER_DELAY = 0.5

OUTTAKE_DELAY = 1
COW_CATCHER_DELAY = 0.5

class MarshAction(GoldRushAction):
    state : int = 0
    pos : np.array
    outtake_delay : DelayAction
    cow_catcher_delay : DelayAction
    calculate_initial : bool = True
    stored_in_pringle : tuple
    stacker : LoadStack

    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.outtake_delay = DelayAction(OUTTAKE_DELAY)
        self.cow_catcher_delay = DelayAction(COW_CATCHER_DELAY)
        self.calculate_initial = True
        self.stacker = LoadStack()

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                # Outtake
                robot.intake.outtake()
                if self.outtake_delay.run(robot):
                    self.state += 1
                    robot.intake.stopIntaking()
            case 1:
                # Move Forward
                if self.calculate_initial:
                    self.pos = robot.drivetrain.position
                    self.calculate_initial = False
                    self.pos[0] += 10
                if robot.drivetrain.driveToPoint(self.pos, 1, .01):
                    self.state += 1
            case 2:
                # Move Backward
                if not self.calculate_initial:
                    self.pos = robot.drivetrain.position
                    self.calculate_initial = True
                    self.pos[0] -= 10
                if robot.drivetrain.driveToPoint(self.pos, 1, .01):
                    self.state += 1
            case 3:
                # Lower Cow Catcher Prep Intake for Marshmallow
                robot.intake.lower_cow_catcher()
                robot.intake.intake_marshmallow()
                # Delay
                if self.cow_catcher_delay.run(robot):
                    self.state += 1
            case 4:
                # Drive to Marshmallow
                if self.drive_to_marsh(robot):
                    self.state += 1
            case 5:
                # Store the Marshmallow that was taken
                if robot.marshmallow.storeMarsh():
                    self.state += 1
            case 6:
                # Decide if we need to load pringle to store more
                if robot.marshmallow.stored_in_revolver.count(MarshmallowColors.EMPTY) == 0:
                    if self.stored_in_pringle.index[MarshmallowColors.EMPTY] == 0:
                        self.stacker.run(robot, (MarshmallowColors.GREEN, MarshmallowColors.WHITE))
                    else:
                        # Finished collecting the Marshmallows
                        return self.nextAction
                else:
                    self.state = 4
                # Add condition for if we run out of time for sweep
        
        return super().run(robot)
    
    def drive_to_marsh(self, robot: Robot) -> bool:
        """Drives until marshmallow is taken into intake

        Args:
            robot (Robot): _description_

        Returns:
            bool: returns true when marshmallow no longer detected (was taken by intake)
        """
        pass
    
class LoadStack(GoldRushAction):
    state : int = 0
    index : int = 0
    delay_pringle : DelayAction
    delay_loader : DelayAction
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.delay_pringle = DelayAction(PRINGLE_DELAY)
        self.delay_loader = DelayAction(LOADER_DELAY)
        
    def run(self, robot: Robot, stack: tuple) -> GoldRushAction:
        match(self.state):
            case 0:
                # Rotate color to Revolver
                if robot.marshmallow.rotate_revolver(stack[self.index], False):
                    self.state += 1
            case 1:
                # Open Pringle Slightly
                robot.marshmallow.set_pringle(PringleStates.LOAD)
                if(self.delay_pringle.run(robot)):
                    self.state += 1
            case 2:
                # Load into Pringle
                robot.marshmallow.set_loader(True)
                if(self.delay_loader.run(robot)):
                    self.state += 1
            case 3:
                # Tighten Pringle
                robot.marshmallow.set_pringle(PringleStates.TIGHT)
                if(self.delay_pringle.run(robot)):
                    self.state += 1
            case 4:
                # Retract Loader
                robot.marshmallow.set_loader(False)
                if(self.delay_loader.run(robot)):
                    if self.index + 1 < len(stack):
                        self.index += 1
                        self.state = 0
                    else:
                        return self.nextAction
        
        return super().run(robot)
    

#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations