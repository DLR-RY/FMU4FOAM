from pyfmi import load_fmu

#Load the FMU
model = load_fmu('Example.fmu')

model.set("port",6006)

# model.set_integer([1], [5005])

model.set_real([2,3], [2.5,3.5])

model.set_boolean([4,5], [True, True])

model.set_integer([6,7], [2,3])

model.set_string([8,9], ["test1","test2"])

res = model.simulate(final_time=0.01,options={"result_handling":"csv"})

