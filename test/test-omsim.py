from pyfmi import load_fmu

#Load the FMU
model = load_fmu('Example.fmu')

res = model.simulate(final_time=0.1)