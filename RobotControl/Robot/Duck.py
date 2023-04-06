from enum import Enum
from adafruit_pca9685 import PCA9685


class DuckArmLocations(Enum):
    AGITATOR = 0
    STORE1 = 1
    STORE2 = 2
    STORE3 = 3
    PLACE2 = 4
    PLACE3 = 5

class Duck:
    def __init__(self, pca: PCA9685) -> None:
        pass

    def prepDuckBag(self) -> None:
        '''servo move to prep'''
        pass

    def deployDuckBag(self) -> None:
        '''servo move to deploy position'''
        pass
    
    def agitate(self, agitate: bool) -> None:
        '''agitate or stop agitating based on
            
            param agitate : bool
           '''
        pass
    
    def square(self) -> bool:
        '''align the agitator in the pickup position

        return True when done'''
        pass

    def duckArm(self, loc: DuckArmLocations, joint: int) -> None:
        '''move a specified joint (1, 2, 3) to the desired location'''
        pass

    def grabDuck(self, grab : bool) -> None:
        '''grab or release duck based on
        :param grab'''
        pass
