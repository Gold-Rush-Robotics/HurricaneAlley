from GoldRushAction import GoldRushAction
from Robot.Robot import Robot

class PlacementAction(GoldRushAction):
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        
        return super().run(robot)


class PlaceStack(GoldRushAction):
    def run(self, robot: Robot, duck:bool) -> GoldRushAction:
        return self