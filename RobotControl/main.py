from Actions.runActions import getRun, runEachLoop
from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot

RED_THRESHOLD = 200

if __name__ == "__main__":
    robot = Robot()

    color = robot.start_sensor.color_rgb_bytes()
    while color[0] < RED_THRESHOLD:
        color = robot.start_sensor.color_rgb_bytes()

    actionTree: GoldRushAction = getRun()
    while(actionTree):
        runEachLoop(robot)
        actionTree = actionTree.run(robot)
        print(actionTree.description)