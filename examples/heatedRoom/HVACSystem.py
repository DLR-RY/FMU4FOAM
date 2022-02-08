import FMU4FOAM
from OMSimulator import OMSimulator
import json
import os
import pytest


class HVACSystem(FMU4FOAM.FMUBase):

    def __init__(self,endTime,filename):
        super().__init__(endTime,filename)
        self.oms = OMSimulator()
        self.oms.setTempDirectory("./temp/")
        self.oms.newModel("model")
        self.oms.addSystem("model.root", self.oms.system_wc)

        # instantiate FMUs
        self.oms.addSubModel("model.root.system1", "HVACSystem.fmu")

        # simulation settings
        self.oms.setResultFile("model", "HVACSystem.csv")
        self.oms.setStopTime("model", endTime)
        # self.oms.setFixedStepSize("model.root", 1e-7)

        self.oms.instantiate("model")
        self.oms.setReal("model.root.system1.Tin", 298)

        self.oms.initialize("model")

    def setVar(self, key: str, val: float) -> None:
        return self.oms.setReal(key,val)

    def getVar(self, key: str) -> float:
        return self.oms.getReal(key)[0]

    def stepUntil(self,t):
        self.oms.stepUntil("model",t)

    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")



