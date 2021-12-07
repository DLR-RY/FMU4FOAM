import pandas as pd
import matplotlib.pyplot as plt

T = pd.read_csv("HVACSystem.csv",skiprows=1)

colval = T.columns.values.copy()
for idx,col in enumerate(colval):
    col = col.replace(' "model.root.system1.',"")
    col = col.replace('"',"")
    colval[idx] = col
T.columns = colval

T = T[T["time"] > 0.01]

plt.plot(T["time"],T["Tin"],label="Tsensor")
plt.plot(T["time"],T["limPID.u_s"],label="Target")
plt.legend()
plt.title("Target and measured temperature")
plt.xlabel("time [s]")
plt.ylabel("T [K]")
plt.tight_layout()
plt.savefig("results/T.png")


plt.figure()
plt.plot(T["time"],T["mout"],label="mout")
plt.title("massflux")
plt.xlabel("time [s]")
plt.ylabel("mdot [kg/s]")
plt.legend()
plt.tight_layout()
plt.savefig("results/massflux.png")

plt.figure()
plt.plot(T["time"],T["Tout"],label="Tinlet")
plt.legend()
plt.title("Temperature at the inlet")
plt.xlabel("time [s]")
plt.ylabel("T [K]")
plt.tight_layout()
plt.savefig("results/heatflox.png")
# plt.figure()
# plt.plot(T["time"],T["Tout"],label="Tout")
# plt.legend()

plt.figure()
plt.plot(T["time"],T["heatFlowSensor.Q_flow"],label="Q_flow")
plt.legend()
plt.title("heatflow")
plt.xlabel("time [s]")
plt.ylabel("Q [W]")
plt.tight_layout()
plt.savefig("results/heatflox.png")

plt.show()