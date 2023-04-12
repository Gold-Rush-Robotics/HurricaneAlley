from __future__ import annotations
from GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Robot.Duck import DuckArmLocations
from DelayAction import DelayAction
import numpy as np
from constants import *

OUTTAKE_DELAY = 1
COW_CATCHER_DELAY = 0.5

DUCK_AGITATOR_DELAY = 5
DUCK_BAG_DELAY = 1

SWEEPS = [[0,0,0], [1, 1, 1]]

J1_DELAY = 1
J2_DELAY = 1
J3_DELAY = 1
GRAB_DELAY = 1

class DuckAction(GoldRushAction):
    state : int = 0
    pos : np.array
    outtake_delay : DelayAction
    cow_catcher_delay : DelayAction
    duck_agitator_delay : DelayAction
    duck_bag_delay : DelayAction
    grab_delay : DelayAction
    at_duck_bag : bool = False
    stored_ducks : int = 0
    sweepState : int = 0
    previous_priority : int = 0
    totalDelay : DelayAction
    store_state : int = 0
    duck_arm : DuckArm

    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.outtake_delay = DelayAction(OUTTAKE_DELAY)
        self.cow_catcher_delay = DelayAction(COW_CATCHER_DELAY)
        self.duck_agitator_delay = DelayAction(DUCK_AGITATOR_DELAY)
        self.duck_bag_delay = DelayAction(DUCK_BAG_DELAY)
        self.grab_delay = DelayAction(GRAB_DELAY)
        self.totalDelay = DelayAction(30000)
        self.duck_arm = DuckArm()

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                # Prep Intake for Duck
                robot.intake.intake_duck_agitator()
                self.duck_arm.run(robot, DuckArmLocations.ARM_STORAGE)
                # Delay
                if not self.cow_catcher_delay.run(robot):
                    self.state += 1
            case 1:
                # Drive to Duck
                if self.drive_to_duck(robot):
                    self.state += 1
            case 2:
                # Agitate the Duck that was taken
                if not self.at_duck_bag:
                    robot.duck.agitate(True)
                    if self.duck_agitator_delay.run(robot):
                        robot.duck.agitate(False)
                        self.state += 1
                else:
                    self.state += 1
            case 3:
                # Store Duck
                match(self.store_state):
                    case 0:
                        # Use Arm to put duck in Store 1
                        self.duck_arm.run(robot, DuckArmLocations.AGITATOR)
                        robot.duck.grabDuck(True)
                        if self.grab_delay.run(robot):
                            self.store_state += 1
                    case 1:
                        self.duck_arm.run(robot, DuckArmLocations.STORE1)
                        robot.duck.grabDuck(False)
                        self.stored_ducks += 1
                        self.store_state += 1
                        self.duck_arm.run(robot, DuckArmLocations.ARM_STORAGE)
                        self.state = 1
                    case 2:
                        # Use Arm to put duck in Store 2
                        self.duck_arm.run(robot, DuckArmLocations.AGITATOR)
                        robot.duck.grabDuck(True)
                        if self.grab_delay.run(robot):
                            self.store_state += 1
                    case 3:
                        self.duck_arm.run(robot, DuckArmLocations.STORE2)
                        robot.duck.grabDuck(False)
                        self.stored_ducks += 1
                        self.store_state += 1
                        self.duck_arm.run(robot, DuckArmLocations.ARM_STORAGE)
                        self.state = 1
                    case 4:
                        # Keep Duck in Agitator, switch to Bag
                        self.stored_ducks += 1
                        robot.intake.intake_duck_bag()
                        # Delay for Intake to switch to duck bag
                        if self.duck_bag_delay.run(robot):
                            self.store_state += 1
                            self.state = 1
                    case 5:
                        # Keep Track of Ducks Stored in Bag
                        if self.stored_ducks == 10:
                            return self.nextAction
                        else:
                            self.stored_ducks += 1
                            self.state = 1

        if not self.totalDelay.run(robot):
            return self.nextAction
        return self

    def sweep(self, robot: Robot) -> bool:
        if(not self.drive_to_duck(robot)):
            if robot.drivetrain.driveToPoint(SWEEPS[self.sweepState], 3, 0.01):
                self.sweepState += 1
    
    def drive_to_duck(self, robot: Robot) -> bool:
        """Drives until duck is taken into intake
        
        TODO: We gotta filter out pink duck if it's one of the first three seen

        Args:
            robot (Robot): _description_

        Returns:
            bool: returns true when duck no longer detected (was taken by intake)
        """
        ducks_detected = robot.vision.detectDucks()
        priority_ducks : list[tuple[tuple[int,int,float,int], float]] = []
        non_priority_ducks : list[tuple[tuple[int,int,float,int], float]] = []
        for duck in ducks_detected:
            distance_from_intake = np.linalg.norm(np.array([CAMERA_X_CENTER, MAX_Y]) - np.array([duck[1], duck[2]]))
            if duck[2] > COW_CATCHER_Y:
                priority_ducks.append((duck, distance_from_intake))
            else:
                non_priority_ducks.append((duck, distance_from_intake))
        
        priority_ducks.sort(key=lambda x: x[1])
        non_priority_ducks.sort(key=lambda x: x[1])

        self.pos = robot.drivetrain.position

        if priority_ducks:
            marsh_pos = (priority_ducks[0][0][0], priority_ducks[0][0][1])
            heading = np.arctan2(marsh_pos[0], marsh_pos[1])
            mod = np.array(marsh_pos[0], marsh_pos[1], heading)
            point = self.pos + mod
            robot.drivetrain.driveToPoint(point, 1, .01)
        elif non_priority_ducks:
            marsh_pos = (non_priority_ducks[0][0][0], non_priority_ducks[0][0][1])
            heading = np.arctan2(marsh_pos[0], marsh_pos[1])
            mod = np.array(marsh_pos[0], marsh_pos[1], heading)
            point = self.pos + mod
            robot.drivetrain.driveToPoint(point, 1, .01)
        else:
            self.sweep(robot)

        if len(priority_ducks) < self.previous_priority:
            self.previous_priority = len(priority_ducks)
            return True

        self.previous_priority = len(priority_ducks)
        return False
    
class DuckArm(GoldRushAction):
    state : int = 0
    delay1 : DelayAction
    delay2 : DelayAction
    delay3 : DelayAction

    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.delay1 = DelayAction(J1_DELAY)
        self.delay2 = DelayAction(J2_DELAY)
        self.delay3 = DelayAction(J3_DELAY)
    
    def run(self, robot: Robot, position: DuckArmLocations) -> GoldRushAction:
        match(self.state):
            case 0:
                robot.duck.duckArm(position, 3)
                if self.delay3.run(robot):
                    self.state += 1
            case 1:
                robot.duck.duckArm(position, 2)
                if self.delay2.run(robot):
                    self.state += 1
            case 2:
                robot.duck.duckArm(position, 1)
                if self.delay1.run(robot):
                    self.state += 1
        return self