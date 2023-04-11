from Actions.runActions import getRun, runEachLoop
from Actions.GoldRushAction import GoldRushAction
from Robot.Robot import Robot

RED_THRESHOLD = 200

if __name__ == "__main__":
    robot = Robot()

    actionTree: GoldRushAction = getRun()
    while(actionTree):
        color = robot.start_sensor.color_rgb_bytes()
        while color[0] < RED_THRESHOLD:
            color = robot.start_sensor.color_rgb_bytes()
        runEachLoop(robot)
        actionTree = actionTree.run(robot)
        print(actionTree.description)