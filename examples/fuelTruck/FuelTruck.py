# from fmpy import read_model_description, extract
# from fmpy.fmi2 import FMU2Slave
import sys
import fmpy
import FMU4FOAM
from OMSimulator import OMSimulator
import json

class FuelTruck(FMU4FOAM.FMUBase):

    def __init__(self,endTime,filename):
        with open("FMU_uncoupled.json") as f:
            d = json.load(f)
        # create mapping and file
        self.data = [x[1] for x in d["mapping"]]
        with open("results.csv", "w") as f:
            f.write("time,currentTime")
            for item in self.data:
                f.write(f"{item},")
            f.write("\n")

        self.oms = OMSimulator()
        super().__init__(endTime,filename)
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
        #self.oms.setFixedStepSize("model.root", 1e-5)

        self.oms.instantiate("model")
        # self.oms.setReal("model.root.system1.Tin", 298)

        self.oms.initialize("model")

    def setVar(self, key: str, val: float) -> None:
        return self.oms.setReal(key,val)

    def getVar(self, key: str) -> float:
        return self.oms.getReal(key)[0]

    def stepUntil(self,t):
        t_old, _ = self.oms.getTime("model")
        step_size = t - t_old
        self.oms.setFixedStepSize("model.root", step_size)
        self.oms.doStep("model")

        with open("results.csv", "a") as f:
            f.write(f"{t},")
            f.write(f"{t_old},")
            for item in self.data:
                val, _ = self.oms.getReal(item)
                f.write(f"{val},")
            f.write("\n")


    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")

    def run(self,nWrite=1000):
            # simulation loop
        step = (self.endTime - 0)/nWrite
        comm_times = [step*interval for interval in range(nWrite)]
        for t in comm_times:
            #self.setVar("f_in[1]",-100)
            #self.setVar("f_in[2]",1000)
            # self.setVar("t_in[3]",1000)
            self.stepUntil(t)

    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")


if __name__ == "__main__":

    ft = FuelTruck(5.0, "FMU_uncoupled.json")
    ft.run()

# import OFFMU
# from OMSimulator import OMSimulator
# import json
# import os
# import pytest


# class FuelTruck(OFFMU.FMUBase):

#     def __init__(self,endTime,filename):
#         with open("FMU.json") as f:
#             d = json.load(f)
#         # create mapping and file
#         self.data = [x[1] for x in d["mapping"]]
#         with open("results.csv", "w") as f:
#             f.write("time,")
#             for item in self.data:
#                 f.write(f"{item},")
#             f.write("\n")

#         self.oms = OMSimulator()
#         super().__init__(filename,self.oms)
#         self.oms.setTempDirectory("./temp/")
#         self.oms.newModel("model")
#         self.oms.addSystem("model.root", self.oms.system_wc)

#         # instantiate FMUs
#         self.oms.addSubModel("model.root.system1", "FuelTruck.fmu")

#         # simulation settings
#         self.oms.setResultFile("model", "FuelTruck.csv")
#         self.oms.setLoggingInterval("model", 0.01)
#         self.oms.setLoggingLevel(0)
        

#         self.oms.setStopTime("model", endTime)
#         #self.oms.setFixedStepSize("model.root", 1e-5)

#         self.oms.instantiate("model")
#         # self.oms.setReal("model.root.system1.Tin", 298)

#         self.oms.initialize("model")

#     def stepUntil(self,t):
#         t_old, _ = self.oms.getTime("model")
#         step_size = t - t_old
#         self.oms.setFixedStepSize("model.root", step_size)
#         # self.oms.setFixedStepSize("model.root", (t-t_old)/10)
#         # self.oms.stepUntil("model",t)
#         self.oms.doStep("model")

#         with open("results.csv", "a") as f:
#             f.write(f"{t},")
#             for item in self.data:
#                 val, _ = self.oms.getReal(item)
#                 f.write(f"{val},")
#             f.write("\n")

#     def __del__(self):
#         self.oms.terminate("model")
#         self.oms.delete("model")



