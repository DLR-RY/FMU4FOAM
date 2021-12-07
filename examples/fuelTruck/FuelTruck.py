import OFFMU
from OMSimulator import OMSimulator
import json
import os
import pytest


class FuelTruck(OFFMU.FMUBase):

    def __init__(self,endTime,filename):
        with open("FMU.json") as f:
            d = json.load(f)
        # create mapping and file
        self.data = [x[1] for x in d["mapping"]]
        with open("results.csv", "w") as f:
            f.write("time,")
            for item in self.data:
                f.write(f"{item},")
            f.write("\n")

        self.oms = OMSimulator()
        super().__init__(filename,self.oms)
        self.oms.setTempDirectory("./temp/")
        self.oms.newModel("model")
        self.oms.addSystem("model.root", self.oms.system_wc)

        # instantiate FMUs
        self.oms.addSubModel("model.root.system1", "FuelTruck.fmu")

        # simulation settings
        self.oms.setResultFile("model", "FuelTruck.csv")
        self.oms.setLoggingInterval("model", 0.01)
        self.oms.setLoggingLevel(0)
        

        self.oms.setStopTime("model", endTime)
        self.oms.setFixedStepSize("model.root", 1e-5)

        self.oms.instantiate("model")
        # self.oms.setReal("model.root.system1.Tin", 298)

        self.oms.initialize("model")

    def stepUntil(self,t):
        t_old, _ = self.oms.getTime("model")
        # self.oms.setFixedStepSize("model.root", (t-t_old)/10)
        self.oms.stepUntil("model",t)

        with open("results.csv", "a") as f:
            f.write(f"{t},")
            for item in self.data:
                val, _ = self.oms.getReal(item)
                f.write(f"{val},")
            f.write("\n")

    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")



