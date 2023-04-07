from __future__ import annotations

class GoldRushAction:
    description: str
    nextAction : GoldRushAction
    def __init__(self, description: str = __qualname__) -> None:
        self.description = description
    def setNext(self, nextAction: GoldRushAction) -> GoldRushAction:
        nextAction.nextAction = self.nextAction
        self.nextAction = nextAction
        return self.nextAction
    def setLast(self, lastAction: GoldRushAction) -> GoldRushAction:
        if(self.nextAction):
            return self.nextAction.setLast(lastAction)
        else:
            self.nextAction = lastAction
            return self.nextAction
    
    
