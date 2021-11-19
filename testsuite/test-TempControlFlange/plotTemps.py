import pandas as pd
import matplotlib.pyplot as plt

T = pd.read_csv("controlledTemperature.csv",skiprows=1)
print(T.columns)


plt.plot(T["time"],T[' "model.root.system1.Tin"'],label="Tin")
plt.plot(T["time"],T[' "model.root.system1.ramp.y"'],label="Target")
plt.legend()
plt.savefig("T.png")

T["diff"] = abs(T[' "model.root.system1.Tin"']-T[' "model.root.system1.ramp.y"'])
print(T["diff"].max())
plt.figure()
plt.plot(T["time"],T["diff"],label="diff")
plt.legend()
plt.figure()
plt.plot(T["time"],T[' "model.root.system1.Qout"'],label="Qout")
plt.legend()
plt.savefig("Q.png")
# plt.show()