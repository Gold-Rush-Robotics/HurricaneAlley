from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Actions.StartupActions import StartupActions
from Actions.foodActions import FoodAction
from Actions.marshmallowActions import MarshAction
from Actions.duckActions import DuckAction
from Actions.placementActions import PlacementAction

def getRun() -> GoldRushAction:
    tree = StartupActions("Startup")
    tree.setNext(FoodAction("Food Action"))
    return tree
"""\
        .setNext(MarshAction("Marshmellows"))\
        .setNext(DuckAction("Duck sweep"))\
        .setNext(PlacementAction("Placement Time"))\
        .setNext(FireWorksAction("Fireworks"))"""
    
    

def runEachLoop(robot: Robot):
    robot.drivetrain.encoderHandler.update()
    robot.marshmallow.revolver_enc.update()
    robot.drivetrain.updatePosition()


class FireWorksAction(GoldRushAction):
    pass