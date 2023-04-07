from Actions.runActions import getRun, runEachLoop
from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot


if __name__ == "__main__":
    robot = Robot()

    actionTree: GoldRushAction = getRun()
    while(actionTree):
        runEachLoop(robot)
        actionTree = actionTree.run(robot)
        print(actionTree.description)