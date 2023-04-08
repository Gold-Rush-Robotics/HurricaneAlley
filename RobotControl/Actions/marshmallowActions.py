from GoldRushAction import GoldRushAction
from Robot.Robot import Robot
from Robot.Marshmallows import MarshmallowColors, PringleStates
from DelayAction import DelayAction
PRINGLE_DELAY = 0.5
LOADER_DELAY = 0.5

#outtake
#drive forward a foot or so
# drive back a foot or so 
# lower cow catcher
# *begin sweep* 

class MarshAction(GoldRushAction):
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
    

    def run(self, robot: Robot) -> GoldRushAction:
        self.drive_to_marsh(robot)
        robot.intake.intake_marshmallow()
        robot.marhmallow.storeMarsh()
        if robot.marhmallow.stored_in_revolver.count(MarshmallowColors.EMPTY) == 0:
            pass
        
        return super().run(robot)
    
    def drive_to_marsh(self, robot: Robot) -> bool:
        # Drive to where intake picks up marshmallow
        pass
    
class LoadStack(GoldRushAction):
    state : int = 0
    index : int = 0
    delay_pringle : DelayAction
    delay_loader : DelayAction
    def __init__(self, description: str = ...) -> None:
        super().__init__(description)
        self.delay_pringle = DelayAction(PRINGLE_DELAY)
        self.delay_loader = DelayAction(LOADER_DELAY)
        
    def run(self, robot: Robot, stack: tuple) -> GoldRushAction:
        match(self.state):
            case 0:
                # Rotate color to Revolver
                if robot.marhmallow.rotate_revolver(stack[self.index], False):
                    self.state += 1
            case 1:
                # Open Pringle Slightly
                robot.marhmallow.set_pringle(PringleStates.LOAD)
                if(self.delay_pringle.run(robot)):
                    self.state += 1
            case 2:
                # Load into Pringle
                robot.marhmallow.set_loader(True)
                if(self.delay_loader.run(robot)):
                    self.state += 1
            case 3:
                # Tighten Pringle
                robot.marhmallow.set_pringle(PringleStates.TIGHT)
                if(self.delay_pringle.run(robot)):
                    self.state += 1
            case 4:
                # Retract Loader
                robot.marhmallow.set_loader(False)
                if(self.delay_loader.run(robot)):
                    if self.index + 1 < len(stack):
                        self.index += 1
                        self.state = 0
                    else:
                        return self.nextAction
                
#Three tall Statue: base level – white, second level – green, third level – red
#Two Tall Statue: base level – white, second level – green
#with a yellow duck on top on the outside pond locations