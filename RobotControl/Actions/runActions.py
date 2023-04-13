from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Actions.StartupActions import StartupActions
from Actions.foodActions import FoodAction
from Actions.marshmallowActions import MarshAction
from Actions.duckActions import DuckAction
from Actions.placementActions import PlacementAction

def getRun() -> GoldRushAction:
    return StartupActions("Startup")\
        .setNext(FoodAction("Food Action"))\
        .setNext(MarshAction("Marshmellows"))\
        .setNext(DuckAction("Duck sweep"))\
        .setNext(PlacementAction("Placement Time"))\
        .setNext(FireWorksAction("Fireworks"))

def runEachLoop(robot: Robot):
    robot.drivetrain.updatePosition()


class FireWorksAction(GoldRushAction):
    pass