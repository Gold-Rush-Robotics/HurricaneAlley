import numpy as np

def clampRange(start: float, stop: float, num: float) -> float:
    return max(start, min(stop, num))

def reMap(OldValue:float, OldMin:float, OldMax:float, NewMax:float, NewMin:float):
    OldRange = (OldMax - OldMin)  
    NewRange = (NewMax - NewMin)  
    return (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

def normalizeAngle(angle: float) -> float:
    return np.arctan2(np.sin(angle), np.cos(angle))

class PID:
    KP:float
    KI:float
    KD:float
    maximum: float
    minimum: float
    _integral: float = 0
    _previousError: float = 0
    dt: float = 0.01
    def __init__(self, kp: float, ki: float, kd: float, maximum:float, minimum:float) -> None:
        self.KP = kp
        self.KI = ki
        self.KD = kd
        self.maximum = maximum
        self.minimum = minimum
    def calculate(self, goal:float, current:float) -> float:
        error = current - goal

        #proportional
        Pout = self.KP * error

        #integral
        self._integral += error * self.dt
        self._integral = clampRange(-.3, .3, self._integral)
        Iout = self.KI * self._integral

        #derivative
        derivative = (error - self._previousError) / self.dt
        Dout = self.KD * derivative
        
        print(Pout, Iout, Dout)

        output = Pout + Iout + Dout

        output = clampRange(self.minimum, self.maximum, output)

        self._previousError = error

        return output


    
