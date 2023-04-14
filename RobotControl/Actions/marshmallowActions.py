from __future__ import annotations
from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors, PringleStates, AGITATOR_SPEED
from Actions.DelayAction import DelayAction
import numpy as np
from constants import *

PRINGLE_DELAY = 0.5
LOADER_DELAY = 1.0

RELEASE_DELAY = 2.0
COW_CATCHER_DELAY = 0.5

SWEEPS = [[0,0,0], [1, 1, 1]]

class MarshAction(GoldRushAction):
    state : int = 0
    pos : np.array
    release_delay : DelayAction
    cow_catcher_delay : DelayAction
    calculate_initial : bool = True
    stored_in_pringle : tuple
    stacker : LoadStack
    sweepState : int = 0
    previous_priority : int = 0

    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        
        self.cow_catcher_delay = DelayAction(COW_CATCHER_DELAY)
        self.calculate_initial = True
        self.stacker = LoadStack()

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                # Release Intake 
                robot.marshmallow.agitator.run(AGITATOR_SPEED)
                if not self.release_delay.run(robot):
                    self.state += 1
                    robot.marshmallow.agitate(0.0)
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
                # Prep Intake for Marshmallow
                robot.intake.intake_marshmallow()
                # Delay
                if not self.cow_catcher_delay.run(robot):
                    robot.intake.intake_motor.run(INTAKE_MARSH_SPEED)
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
        
        return self

    def sweep(self, robot: Robot) -> bool:
        if(not self.drive_to_marsh(robot)):
            if robot.drivetrain.driveToPoint(SWEEPS[self.sweepState], 3, 0.01):
                self.sweepState += 1
    
    def drive_to_marsh(self, robot: Robot) -> bool:
        """Drives until marshmallow is taken into intake

        Args:
            robot (Robot): _description_

        Returns:
            bool: returns true when marshmallow no longer detected (was taken by intake)
        """
        marshmallows_detected = robot.vision.detectMarshmallows()
        priority_marshmallows : list[tuple[tuple[int,int,float,MarshmallowColors], float]] = []
        non_priority_marshmallows : list[tuple[tuple[int,int,float,MarshmallowColors], float]] = []
        for marshmallow in marshmallows_detected:
            distance_from_intake = np.linalg.norm(np.array([CAMERA_X_CENTER, MAX_Y]) - np.array([marshmallow[1], marshmallow[2]]))
            if marshmallow[2] > COW_CATCHER_Y:
                priority_marshmallows.append((marshmallow, distance_from_intake))
            else:
                non_priority_marshmallows.append((marshmallow, distance_from_intake))
        
        priority_marshmallows.sort(key=lambda x: x[1])
        non_priority_marshmallows.sort(key=lambda x: x[1])

        self.pos = robot.drivetrain.position

        if priority_marshmallows:
            marsh_pos = (priority_marshmallows[0][0][0], priority_marshmallows[0][0][1])
            heading = np.arctan2(marsh_pos[0], marsh_pos[1])
            mod = np.array(marsh_pos[0], marsh_pos[1], heading)
            point = self.pos + mod
            robot.drivetrain.driveToPoint(point, 1, .01)
        elif non_priority_marshmallows:
            marsh_pos = (non_priority_marshmallows[0][0][0], non_priority_marshmallows[0][0][1])
            heading = np.arctan2(marsh_pos[0], marsh_pos[1])
            mod = np.array(marsh_pos[0], marsh_pos[1], heading)
            point = self.pos + mod
            robot.drivetrain.driveToPoint(point, 1, .01)
        else:
            self.sweep(robot)

        if len(priority_marshmallows) < self.previous_priority:
            self.previous_priority = len(priority_marshmallows)
            return True

        self.previous_priority = len(priority_marshmallows)
        return False
    
class LoadStack(GoldRushAction):
    state : int = 0
    index : int = 0
    delay_pringle : DelayAction
    delay_loader : DelayAction
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.delay_pringle = DelayAction(PRINGLE_DELAY)
        self.delay_loader = DelayAction(LOADER_DELAY)
        
    def run(self, robot: Robot, stack: tuple[MarshmallowColors], flag:bool) -> GoldRushAction:
        print(self.state)
        if(flag):
            return self.nextAction
        match(self.state):
            case 0:
                # Rotate color to Revolver
                if robot.marshmallow.rotate_revolver(stack[self.index], False):
                    return self.nextAction
                    self.state += 1
            case 1:
                # Open Pringle Slightly
                robot.marshmallow.set_pringle(PringleStates.LOAD)
                if not (self.delay_pringle.run(robot)):
                    self.delay_pringle.endTime = -1
                    self.state += 1
            case 2:
                # Load into Pringle
                robot.marshmallow.set_loader(True)
                if not (self.delay_loader.run(robot)):
                    self.delay_loader.endTime = -1
                    self.state += 1
            case 3:
                # Tighten Pringle
                robot.marshmallow.set_pringle(PringleStates.TIGHT)
                if not (self.delay_pringle.run(robot)):
                    self.delay_pringle.endTime = -1
                    self.state += 1
            case 4:
                # Retract Loader
                robot.marshmallow.set_loader(False)
                if not (self.delay_loader.run(robot)):
                    self.delay_loader.endTime = -1
                    if self.index < len(stack)-1:
                        self.index += 1
                        self.state = 0
                    else:
                        return self.nextAction
        
        return self
    

#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations