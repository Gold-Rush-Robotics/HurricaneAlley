from enum import Enum
from Hardware.actuators import Servo, PWMMotor
from adafruit_pca9685 import PCA9685
from microcontroller import Pin
import digitalio

class DuckArmLocations(Enum):
    AGITATOR = 0
    STORE1 = 1
    STORE2 = 2
    PLACE2 = 3
    PLACE3 = 4
    ARM_STORAGE = 5
    INITIAL_STORAGE = 5
    
DUCK_BAG_PREP = 0
DUCK_BAG_DEPLOY = 0

DUCK_AGITATOR_SPEED = 0.5

HOME_AGITATOR_SPEED = 0.1

AGITATOR_POS = [0, 0, 0]
STORE_1_POS = [0, 0, 0]
STORE_2_POS = [0, 0, 0]
PLACE_2_POS = [0, 0, 0]
PLACE_3_POS = [0, 0, 0] 
ARM_STORAGE_POS = [0, 0, 0] 
INITIAL_STORAGE_POS = [0, 0, 0]

GRAB_DUCK_POS = 0
RELEASE_DUCK_POS = 0

class Duck:
    j1 : Servo
    j2 : Servo
    j3 : Servo
    
    duck_bag : Servo
    duck_gripper : Servo
    duck_agitator : PWMMotor
    
    duck_bag_deployed : bool = False
    duck_bag_prepped  : bool = False
    
    home_sensor : digitalio.DigitalInOut
    
    home : Pin  = Pin(18) # 18 on the pi hat
    dir : Pin = Pin(0)
    
    def __init__(self, pca: PCA9685) -> None:
        self.j1 = Servo(pca, 7, 0, 180)
        self.j2 = Servo(pca, 8, 0, 180)
        self.j3 = Servo(pca, 9, 0, 180)
        self.duck_bag = Servo(pca, 4, 0, 180)
        self.duck_gripper = Servo(pca, 10, 0, 180)
        
        self.duck_agitator = PWMMotor(15, self.dir, pca)
        
        self.home_sensor = digitalio.DigitalInOut(self.home)
        self.home_sensor.direction = digitalio.Direction.INPUT
        self.home_sensor.pull = digitalio.Pull.UP

    def prepDuckBag(self) -> None:
        """servo move to prep
        """
        if not self.duck_bag_prepped:
            self.duck_bag.run(DUCK_BAG_PREP)
            self.duck_bag_prepped = True
        else:
            pass

    def deployDuckBag(self) -> None:
        '''servo move to deploy position'''
        if not self.duck_bag_deployed:
            self.duck_bag.run(DUCK_BAG_DEPLOY)
            self.duck_bag_deployed = True
        else:
            pass
    
    def agitate(self, agitate: bool) -> None:
        """agitate or stop agitating

        Args:
            agitate (bool): state agitator should be in
        """
        if agitate:
            self.duck_agitator.run(DUCK_AGITATOR_SPEED)
        else:
            self.duck_agitator.run(0)
    
    def home_agitator(self) -> bool:
        """align the agitator in the pickup position

        Returns:
            bool: true when finished
        """
        if self.home_sensor.value:
            self.duck_agitator.run(0)
            return True
        else:
            self.duck_agitator.run(HOME_AGITATOR_SPEED)
            return False

    def duckArm(self, loc: DuckArmLocations, joint: int) -> None:
        '''move a specified joint (1, 2, 3) to the desired location'''
        match(loc):
            case DuckArmLocations.AGITATOR:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(AGITATOR_POS[i])
                            self.j2.run(AGITATOR_POS[i])
                            self.j3.run(AGITATOR_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(AGITATOR_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(AGITATOR_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(AGITATOR_POS[2])
            
            case DuckArmLocations.STORE1:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(STORE_1_POS[i])
                            self.j2.run(STORE_1_POS[i])
                            self.j3.run(STORE_1_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(STORE_1_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(STORE_1_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(STORE_1_POS[2])
            
            case DuckArmLocations.STORE2:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(STORE_2_POS[i])
                            self.j2.run(STORE_2_POS[i])
                            self.j3.run(STORE_2_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(STORE_2_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(STORE_2_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(STORE_2_POS[2])
                        
            case DuckArmLocations.PLACE2:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(PLACE_2_POS[i])
                            self.j2.run(PLACE_2_POS[i])
                            self.j3.run(PLACE_2_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(PLACE_2_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(PLACE_2_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(PLACE_2_POS[2])
                        
            case DuckArmLocations.PLACE3:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(PLACE_3_POS[i])
                            self.j2.run(PLACE_3_POS[i])
                            self.j3.run(PLACE_3_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(PLACE_3_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(PLACE_3_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(PLACE_3_POS[2])
            case DuckArmLocations.ARM_STORAGE:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(ARM_STORAGE_POS[i])
                            self.j2.run(ARM_STORAGE_POS[i])
                            self.j3.run(ARM_STORAGE_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(ARM_STORAGE_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(ARM_STORAGE_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(ARM_STORAGE_POS[2])
            case DuckArmLocations.INITIAL_STORAGE:
                match(joint):
                    case -1:
                        # All Joints at Once
                        for i in range(3):
                            self.j1.run(INITIAL_STORAGE_POS[i])
                            self.j2.run(INITIAL_STORAGE_POS[i])
                            self.j3.run(INITIAL_STORAGE_POS[i])
                    case 1:
                        # Joint 1
                        self.j1.run(INITIAL_STORAGE_POS[0])
                    case 2:
                        # Joint 2
                        self.j2.run(INITIAL_STORAGE_POS[1])
                    case 3:
                        # Joint 3
                        self.j3.run(INITIAL_STORAGE_POS[2])
            

    def grabDuck(self, grab : bool) -> None:
        """Grab or relase duck gripper

        Args:
            grab (bool): true to grab duck
        """
        if grab:
            self.duck_gripper.run(GRAB_DUCK_POS)
        else:
            self.duck_gripper.run(RELEASE_DUCK_POS)
