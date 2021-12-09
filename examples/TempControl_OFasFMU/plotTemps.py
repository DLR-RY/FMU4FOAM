import pandas as pd
import matplotlib.pyplot as plt

T = pd.read_csv("results.csv")
print(T.columns)


plt.plot(T["time"],T['Tin'],label="Tin")
plt.plot(T["time"],T['ramp.y'],label="Target")
plt.legend()

T["diff"] = abs(T['Tin']-T['ramp.y'])
print(T["diff"].max())
plt.figure()
plt.plot(T["time"],T["diff"],label="diff")
plt.legend()
plt.figure()
plt.plot(T["time"],T['Qout'],label="Qout")
plt.legend()
plt.show()