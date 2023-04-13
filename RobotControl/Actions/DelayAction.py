from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from datetime import datetime


class DelayAction(GoldRushAction):
    endTime: float

    def __init__(self, delayTime: float, description: str = ..., ) -> None:
        """Construct Delay Action

        Args:
            delayTime (float): Seconds to delay
            description (str, optional): description to print. Defaults to ....
        """
        super().__init__(description)
        self.endTime = (datetime.utcnow() / 1000) + delayTime

    def run(self, robot: Robot) -> GoldRushAction:
        if((datetime.utcnow() / 1000) >= self.endTime):
            return self.nextAction
        else:
            return self
        
