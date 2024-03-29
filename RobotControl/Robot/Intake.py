from Hardware.actuators import GRRRoboClaw, Servo, PWMMotor
from adafruit_pca9685 import PCA9685
from constants import *

AB_SEPARATOR_AGITATOR = 0
AB_SEPARATOR_BAG = 0

COW_CATCHER_LOWERED = 0
COW_CATCHER_MID     = 0
COW_CATCHER_RAISED  = 0

OUTTAKE_SPEED = -1
COW_CATCHER_SPEED = 1

class Intake:
    
    intake_motor : GRRRoboClaw
    cow_catcher_motor : PWMMotor
    ab_separator_servo : Servo
    dm_separator_servo : Servo
    cow_catcher_servo  : Servo

    def __init__(self, pca: PCA9685) -> None:
        self.intake_motor = GRRRoboClaw(pca, 0x00, True)
        self.cow_catcher_motor = PWMMotor(0, 0, pca)
        self.agitator_bag_servo = Servo(pca, 0, 0, 180)
        self.dm_separator_servo = Servo(pca, 0, 0, 180)
        self.cow_catcher_servo  = Servo(pca, 0, 0, 180)

    def intake_marshmallow(self) -> None:
        """Sets cow_catcher and dm_servo to intake a marshmallow
        """
        self.dm_separator_servo.run(DM_SEPARATOR_MARSHMALLOW)
        self.lower_cow_catcher()
    
    def stopIntaking(self) -> None:
        self.intake_motor.run(0.0)
        self.cow_catcher_motor.run(0.0)
        self.raise_cow_catcher()
        
    def intake_duck_agitator(self) -> None:
        self.raise_cow_catcher()
        self.intake_motor.run(INTAKE_DUCK_AG_SPEED)
        self.dm_separator_servo.run(DM_SEPARATOR_DUCK)
        self.ab_separator_servo.run(AB_SEPARATOR_AGITATOR)

    def intake_duck_bag(self) -> None:
        self.raise_cow_catcher()
        self.intake_motor.run(INTAKE_DUCK_BAG_SPEED)
        self.dm_separator_servo.run(DM_SEPARATOR_DUCK)
        self.ab_separator_servo.run(AB_SEPARATOR_BAG)

    def raise_cow_catcher(self) -> None:
        self.cow_catcher_motor.run(0.0)
        self.cow_catcher_servo.run(COW_CATCHER_MID)

    def lower_cow_catcher(self) -> None:
        self.cow_catcher_servo.run(COW_CATCHER_LOWERED)
        self.cow_catcher_motor.run(COW_CATCHER_SPEED)
        
    def outtake(self) -> None:
        self.raise_cow_catcher()
        self.intake_motor.run(OUTTAKE_SPEED)