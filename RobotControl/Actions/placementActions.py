from GoldRushAction import GoldRushAction
from DelayAction import DelayAction
from Robot.Robot import Robot
from Robot.Marshmallows import PringleStates


class PlaceDuck(GoldRushAction):
    def run(self, robot: Robot) -> GoldRushAction:
        return self

class PlaceStack(GoldRushAction):
    state: int
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


    def run(self, robot: Robot, duck:bool) -> GoldRushAction:
        match(self.state):
            case 0:
                robot.marshmallow.place_pringle(True)
                robot.marshmallow.set_pringle(PringleStates.TIGHT)
                if not self.placeDelay.run(robot):
                    self.state = 1
            case 1:
                robot.marshmallow.set_pringle(PringleStates.OPEN)
                if not self.openDelay.run(robot):
                    self.state = 2 if duck else 10
            case 2:
                if not 
            case 10:
                robot.marshmallow.inPringle = 0
        return self



class PlacementAction(GoldRushAction):
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        
        return super().run(robot)