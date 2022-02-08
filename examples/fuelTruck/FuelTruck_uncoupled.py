""" This example demonstrates how to use the FMU.get*() and FMU.set*() functions
 to set custom input and control the simulation """
from fmpy import read_model_description, extract
from fmpy.fmi2 import FMU2Slave
import shutil
import FMU4FOAM


class FuelTruck(FMU4FOAM.FMUBase):
    # class FuelTruck:
    def __init__(self, endTime, filename):
        super().__init__(endTime, filename)
        # define the model name and simulation parameters
        fmu_file = "FuelTruck.fmu"
        self.start_time = 0.0

        # read the model description
        self.model_description = read_model_description(fmu_file)

        # collect the value references
        self.vrs = {}
        for variable in self.model_description.modelVariables:
            self.vrs[variable.name] = variable.valueReference

        self.outputKeys = [
            k
            for k, v in self.coupleData.FMU_to_OF.items()
            if v in self.coupleData.to_OF
        ]
        self.inputKeys = [
            k
            for k, v in self.coupleData.FMU_to_OF.items()
            if v in self.coupleData.from_OF
        ]
        self.vr_outputs = [self.vrs[o] for o in self.outputKeys]
        self.vr_inputs = [self.vrs[i] for i in self.inputKeys]

        with open("results_uncoupled.csv", "w") as f:
            f.write("time,")
            for item in self.outputKeys:
                f.write(f"{item},")
            for item in self.inputKeys:
                f.write(f"{item},")
            f.write("\n")
        # extract the FMU
        self.unzipdir = extract(fmu_file)

        self.fmu = FMU2Slave(
            guid=self.model_description.guid,
            unzipDirectory=self.unzipdir,
            modelIdentifier=self.model_description.coSimulation.modelIdentifier,
            instanceName="FuelTruck",
        )

        # initialize
        self.fmu.instantiate()
        self.fmu.setupExperiment(startTime=self.start_time)
        self.fmu.enterInitializationMode()
        self.fmu.exitInitializationMode()

        self.time = self.start_time
        self.dt = 0

    def stepUntil(self, t):
        self.dt  = t - self.time
        self.fmu.doStep(
            currentCommunicationPoint=self.time, communicationStepSize=self.dt 
        )
        self.time = self.time + self.dt 

        with open("results_uncoupled.csv", "a") as f:
            f.write(f"{t},")
            for item in self.vr_outputs:
                val = self.fmu.getReal([item])
                f.write(f"{val[0]},")
            for item in self.vr_inputs:
                val = self.fmu.getReal([item])
                f.write(f"{val[0]},")
            f.write("\n")

    def run(self,nWrite=1000):
        # simulation loop
        step = (self.endTime - 0)/nWrite
        comm_times = [step*interval for interval in range(nWrite)]
        for t in comm_times:
            self.stepUntil(t)

    def setVar(self, key: str, val: float) -> None:
        # set the forces and torque from openfoam to zero
        # to model uncoupled results
        val = 0
        self.fmu.setReal([self.vrs[key]], [val])

    def getVar(self, key: str) -> float:
        return self.fmu.getReal([self.vrs[key]])[0]

    def __del__(self):

        self.fmu.terminate()
        self.fmu.freeInstance()

        # clean up
        shutil.rmtree(self.unzipdir, ignore_errors=True)

        # convert the results to a structured NumPy array


if __name__ == "__main__":

    ft = FuelTruck(5.0, "FMU.json")
    ft.run()
