from GoldRushAction import GoldRushAction
from Robot.Robot import Robot

class FoodAction(GoldRushAction):
    state: int = 0
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        match(self.state):
            case 0:
                robot.drivetrain.driveToPoint([])