from Actions.GoldRushAction import GoldRushAction
from Robot.Marshmallows import PringleStates
from Robot.Robot import Robot

import numpy as np

NUM_SAMPLES = 150

class StartupActions(GoldRushAction):
    samples = []
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.state =0 
    

    def run(self, robot: Robot) -> GoldRushAction:
        print(self.state)
        match(self.state):
            case 0: 
                robot.marshmallow.set_pringle(PringleStates.LOAD)
                if robot.duck.home_sensor.value:
                    robot.marshmallow.set_pringle(PringleStates.TIGHT)
                    self.state += 1
            case 1:
                for i in range(NUM_SAMPLES):
                    self.samples.append(robot.start_color_sensor.lux)
                self.data = np.array(self.samples)
                self.std = np.std(self.data)
                self.mean = np.average(self.data)
                self.state += 1
            case 2:
                value = robot.start_color_sensor.lux
                if not np.isclose(value, self.mean, atol=2*self.std):
                    
                    return self.nextAction
        return self