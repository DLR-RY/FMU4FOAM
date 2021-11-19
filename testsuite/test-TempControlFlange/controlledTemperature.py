import FMU4FOAM
from OMSimulator import OMSimulator
import json
import os
import pytest


class controlledTemperature(FMU4FOAM.FMUBase):

    def __init__(self,endTime,filename):
        self.oms = OMSimulator()
        super().__init__(filename,self.oms)
        self.oms.setTempDirectory("./temp/")
        self.oms.newModel("model")
        self.oms.addSystem("model.root", self.oms.system_wc)

        # instantiate FMUs
        self.oms.addSubModel("model.root.system1", "ControlledTemperatureCoupled.fmu")

        # simulation settings
        self.oms.setResultFile("model", "controlledTemperature.csv")
        self.oms.setStopTime("model", endTime)
        # self.oms.setFixedStepSize("model.root", 1e-7)

        self.oms.instantiate("model")
        self.oms.setReal("model.root.system1.Tin", 298)
        self.oms.setReal("model.root.system1.dTin", 0)

        self.oms.initialize("model")

    def stepUntil(self,t):
        self.oms.setReal("model.root.system1.dTin", 0)
        self.oms.stepUntil("model",t)

    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")



