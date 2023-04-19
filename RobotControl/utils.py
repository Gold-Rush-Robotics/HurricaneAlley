import numpy as np
import numpy.typing as npTyping

def clampRange(start: float, stop: float, num: float) -> float:
    return max(start, min(stop, num))

def reMap(OldValue:float, OldMin:float, OldMax:float, NewMax:float, NewMin:float):
    OldRange = (OldMax - OldMin)  
    NewRange = (NewMax - NewMin)  
    return (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

def normalizeAngle(angle: float) -> float:
    return np.arctan2(np.sin(angle), np.cos(angle))

def unitVectorinator(vector:npTyping.ArrayLike) -> npTyping.ArrayLike:
    return ( vector / np.linalg.norm(vector) )

class PID:
    KP:float
    KI:float
    KD:float
    maximum: float
    minimum: float
    _integral: float = 0
    _previousError: float = 0
    dt: float = 0.01 # TODO run a timer each loop to figure out exactly what dt should be
    def __init__(self, kp: float, ki: float, kd: float, maximum:float, minimum:float, integralAntiWindup:float = 1.0) -> None:
        self.KP = kp
        self.KI = ki
        self.KD = kd
        self.maximum = maximum
        self.minimum = minimum
        self._integralAntiWindup = integralAntiWindup
    def calculate(self, goal:float, current:float) -> float:
        error = current - goal

        Iout = 0
        #proportional
        Pout = self.KP * error

        #integral
        if(self.KI):
            self._integral += error * self.dt
            self._integral = clampRange(-self._integralAntiWindup / self.KI, self._integralAntiWindup / self.KI, self._integral)
            Iout = self.KI * self._integral

        #derivative
        derivative = (error - self._previousError) / self.dt
        Dout = self.KD * derivative
        
        print(Pout, Iout, Dout)

        output = Pout + Iout + Dout

        output = clampRange(self.minimum, self.maximum, output)

        self._previousError = error

        return output


    
