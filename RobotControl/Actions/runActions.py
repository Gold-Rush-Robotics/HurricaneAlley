from GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from StartupActions import StartupActions
from foodActions import FoodAction
from marshmallowActions import MarshAction
from duckActions import DuckAction
from placementActions import PlacementAction

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