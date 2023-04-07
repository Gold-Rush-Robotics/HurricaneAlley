from enum import Enum
from Hardware.actuators import GRRRoboClaw, Servo
from adafruit_pca9685 import PCA9685
import numpy as np
import math

DM_SEPARATOR_MARSHMALLOW = 0 
DM_SEPARATOR_DUCK = 0

AB_SEPARATOR_AGITATOR = 0
AB_SEPARATOR_BAG = 0

COW_CATCHER_LOWERED = 0
COW_CATCHER_RAISED = 0

class Intake:
    
    intake_motor : GRRRoboClaw
    ab_separator_servo : Servo
    dm_separator_servo : Servo
    cow_catcher_servo  : Servo

    def __init__(self, pca: PCA9685) -> None:
        self.intake_motor = GRRRoboClaw(pca, 0x00, True)
        self.agitator_bag_servo = Servo(pca, 0, 0, 180)
        self.dm_separator_servo = Servo(pca, 0, 0, 180)
        self.cow_catcher_servo  = Servo(pca, 0, 0, 180)

    def intake_marshmallow(self) -> None:
        self.intake_motor.run(0.66)
        self.dm_separator_servo.run(DM_SEPARATOR_MARSHMALLOW)
        
    def intake_duck_agitator(self) -> None:
        self.intake_motor.run(0.83)
        self.dm_separator_servo.run(DM_SEPARATOR_DUCK)
        self.ab_separator_servo.run(AB_SEPARATOR_AGITATOR)

    def intake_duck_bag(self) -> None:
        self.intake_motor.run(1)
        self.dm_separator_servo.run(DM_SEPARATOR_DUCK)
        self.ab_separator_servo.run(AB_SEPARATOR_BAG)

    def raise_cow_catcher(self) -> None:
        self.cow_catcher_servo.run(COW_CATCHER_RAISED)

    def lower_cow_catcher(self) -> None:
        self.cow_catcher_servo.run(COW_CATCHER_LOWERED)