from Hardware.actuators import GRRRoboClaw
from adafruit_pca9685 import PCA9685


class Drivetrain:
    fl : GRRRoboClaw
    fr : GRRRoboClaw
    bl : GRRRoboClaw
    br : GRRRoboClaw
    pca: PCA9685
    
    def __init__(self, pca: PCA9685) -> None:
        fl = GRRRoboClaw(pca, 0x00, True)
        fr = GRRRoboClaw(pca, 0x00, True)
        bl = GRRRoboClaw(pca, 0x00, True)
        br = GRRRoboClaw(pca, 0x00, True)
    
    

    def driveM(self, fl: float, fr: float, bl: float, br: float) -> None:
        self.fl.run(fl)
        self.fr.run(fr)
        self.bl.run(bl)
        self.br.run(br)