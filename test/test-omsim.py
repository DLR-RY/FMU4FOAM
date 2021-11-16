from pyfmi import load_fmu

#Load the FMU
model = load_fmu('Example.fmu')

model.set_real([0,1], [2.5,3.5])

model.set_boolean([2,3], [True, True])

model.set_integer([4,5], [2,3])

model.set_string([6,7], ["test1","test2"])

res = model.simulate(final_time=0.01,options={"result_handling":"csv"})
