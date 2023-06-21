from pyPS4Controller.controller import Controller
from pyPS4Controller.event_mapping.Mapping3Bh2b import Mapping3Bh2b
from Robot.Robot import Robot
import sys

MAXNUM = 32767

class robot:
    r = Robot()
    lx = 0
    ly = 0
    rx = 0
    ry = 0
    
    def move():
        forward =-robot.ly / MAXNUM
        strafe = robot.lx / MAXNUM
        rotate = -robot.rx / MAXNUM
        robot.r.drivetrain.drivePow(forward, strafe, rotate)
    

class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)
    
    def on_L3_left(self, value):
        print("LEFT")
        robot.lx = value
        robot.move()
        return super().on_L3_left(value)
    def on_L3_right(self, value):
        robot.lx = value
        robot.move()
        return super().on_L3_right(value)
    def on_L3_x_at_rest(self):
        robot.lx = 0
        robot.move()
        return super().on_L3_x_at_rest()
    def on_L3_y_at_rest(self):
        robot.ly = 0
        robot.move()
        return super().on_L3_y_at_rest()
    def on_L3_up(self, value):
        robot.ly = value
        robot.move()
        return super().on_L3_up(value)
    def on_L3_down(self, value):
        robot.ly = value
        robot.move()
        return super().on_L3_down(value)
    
    
    
    def on_R3_left(self, value):
        robot.rx = value
        robot.move()
        return super().on_R3_left(value)
    def on_R3_right(self, value):
        robot.rx = value
        robot.move()
        return super().on_R3_right(value)
    def on_R3_x_at_rest(self):
        robot.rx = 0
        robot.move()
        return super().on_R3_x_at_rest()
    def on_R3_y_at_rest(self):
        robot.rx = 0
        robot.move()
        return super().on_R3_y_at_rest()
    def on_R3_up(self, value):
        robot.ry = value
        robot.move()
        return super().on_R3_up(value)
    def on_R3_down(self, value):
        robot.ry = value
        robot.move()
        return super().on_R3_down(value)


class MyEventDefinition(Mapping3Bh2b):

    def __init__(self, **kwargs):
        Mapping3Bh2b.__init__(self, **kwargs)
    # each overloaded function, has access to:
    # - self.button_id
    # - self.button_type
    # - self.value
    # - self.overflow
    # use those variables to determine which button is being pressed
    def x_pressed(self):
        return self.button_id == 1 and self.button_type == 1 and self.value == 1

    def x_released(self):
        return self.button_id == 1 and self.button_type == 1 and self.value == 0

    def on_playstation_button_pressed(self):
        return self.button_id == 12 and self.button_type == 1 and self.value == 1

    def on_playstation_button_released(self):
        return self.button_id == 12 and self.button_type == 1 and self.value == 1
    
    def on_L3_pressed(self):
        return self.button_id == 10 and self.button_type == 1 and self.value == 1

    def on_L3_released(self):
        return self.button_id == 10 and self.button_type == 1 and self.value == 0
        

controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=True, event_definition=MyEventDefinition)
#controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
# you can start listening before controller is paired, as long as you pair it within the timeout window

def disconnect():
    robot.r.clear()
    sys.exit(0)
    

controller.listen(timeout=60, on_disconnect=disconnect)
