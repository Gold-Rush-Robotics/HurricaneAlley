from Actions.GoldRushAction import GoldRushAction
from Actions.DelayAction import DelayAction
from Robot.Robot import Robot
from Robot.Marshmallows import PringleStates
import time


class PlaceDuck(GoldRushAction):
    def run(self, robot: Robot) -> GoldRushAction:
        return self

class PlaceStack(GoldRushAction):
    state: int = 0
    placeDelay: DelayAction
    openDelay: DelayAction
    raiseDelay: DelayAction
    closeDelay: DelayAction
    placeDuck: PlaceDuck

    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.placeDelay = DelayAction(2, self.description + " Pringle going down")
        self.openDelay = DelayAction(1, self.description + " Opening Pringle")
        self.raiseDelay = DelayAction(2, self.description + " Raising Pringle")
        self.closeDelay = DelayAction(1, self.description + " Closing Pringle")
        self.placeDuck = PlaceDuck(self.description + " Placing Duck")

    def run(self, robot: Robot, backUPX: bool = False) -> GoldRushAction:
        match(self.state):
            case 0:
                robot.marshmallow.place_pringle(True)
                robot.marshmallow.set_pringle(PringleStates.TIGHT)
                if not self.placeDelay.run(robot):
                    self.state = 1
            case 1:
                robot.marshmallow.set_pringle(PringleStates.OPEN)
                if not self.openDelay.run(robot):
                    self.state = 3
            case 3:
                robot.drivetrain.drivePow(.5, 0, 0)
                time.sleep(.5)
                robot.drivetrain.drivePow(0, 0, 0)
                robot.marshmallow.set_pringle(PringleStates.TIGHT)
                robot.marshmallow.place_pringle(False)
                time.sleep(1)
                return self.nextAction
        return self



class PlacementAction(GoldRushAction):
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        
        return super().run(robot)