# from pyfmi import load_fmu

# #Load the FMU
# model = load_fmu('Example.fmu')

# res = model.simulate(final_time=0.01)

from OMSimulator import OMSimulator

oms = OMSimulator()
oms.setTempDirectory("./temp/")
oms.newModel("model")
oms.addSystem("model.root", oms.system_wc)

# instantiate FMUs
oms.addSubModel("model.root.system1", "Example.fmu")
# oms.addSubModel("model.root.system2", "FMUs/System2.fmu")

# add connections
# oms.addConnection("model.root.system1.y", "model.root.system2.u")
# oms.addConnection("model.root.system2.y", "model.root.system1.u")

# simulation settings
oms.setResultFile("model", "results.csv")
oms.setStopTime("model", 0.1)
oms.setFixedStepSize("model.root", 0.01)

oms.instantiate("model")
oms.setReal("model.root.system1.r_in", 2.5)
oms.setReal("model.root.system1.r_out", 3.5)

oms.setInteger("model.root.system1.i_in", 2)
oms.setInteger("model.root.system1.i_out", 3)

oms.setBoolean("model.root.system1.b_in", True)
oms.setBoolean("model.root.system1.b_out", True)

oms.initialize("model")
oms.doStep("model")
oms.terminate("model")
oms.delete("model")