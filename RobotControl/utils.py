def clampRange(start: float, stop: float, num: float) -> float:
    return max(start, min(stop, float))

def reMap(OldValue:float, OldMin:float, OldMax:float, NewMax:float, NewMin:float):
    OldRange = (OldMax - OldMin)  
    NewRange = (NewMax - NewMin)  
    return (((OldValue - OldMin) * NewRange) / OldRange) + NewMin