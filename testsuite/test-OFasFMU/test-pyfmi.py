from pyfmi import load_fmu
from pyfmi.master import Master
import pandas as pd

sub_system1 = load_fmu("Example.fmu")

sub_system2 = load_fmu("ControlledTemperatureCoupled.fmu")
models = [sub_system1, sub_system2]

connections = [ (sub_system1 ,"Tout",sub_system2 ,"Tin"),
                (sub_system1 ,"dTout",sub_system2 ,"dTin"),    
                (sub_system2 ,"Qout",sub_system1 ,"Qin")]

sub_system2.set('Tin',300.0)
sub_system2.set('constantVoltage.V',50.0)

sub_system1.set("port",6006)
sub_system1.set("outputPath","Example")

master_simulator = Master (models, connections)
opts = master_simulator.simulate_options()
opts["step_size"] = 0.005
opts["result_handling"] = "file"

res = master_simulator.simulate(start_time = 0.0,final_time = 5.0,options=opts)#, input = input_object)

results = pd.DataFrame()
results["time"] = res[sub_system2]["time"]
results["ramp.y"] = res[sub_system2]["ramp.y"]
results["Tin"] = res[sub_system2]["Tin"]
results["Qout"] = res[sub_system2]["Qout"]
results.to_csv("results.csv",index=False)
