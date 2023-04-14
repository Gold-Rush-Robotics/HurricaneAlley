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
        self.endTime = -1
        self.delayTime = delayTime
    def run(self, robot: Robot) -> GoldRushAction:
        if(self.endTime == -1):
            self.endTime = (datetime.utcnow().timestamp()) + (self.delayTime * 2)

        curr = (datetime.utcnow().timestamp())
        print(curr, self.endTime)
        if( curr >= self.endTime):
            return self.nextAction
        else:
            return self
        
