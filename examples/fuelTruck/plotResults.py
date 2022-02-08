import pandas as pd
import matplotlib.pyplot as plt

coupled = pd.read_csv("results_coupled.csv")
un_coupled = pd.read_csv("results_uncoupled.csv")

colval = un_coupled.columns.values.copy()
for idx,col in enumerate(colval):
    col = col.replace('model.root.system1.',"")
    col = col.replace('"',"")
    colval[idx] = col
un_coupled.columns = colval
print(un_coupled)
un_coupled = un_coupled[un_coupled["time"] > 0.01]
coupled = coupled[coupled["time"] > 0.01]

plt.plot(coupled["time"],coupled["pos_out[1]"],color="C0",linestyle="solid",label="position x (coupled)")
plt.plot(coupled["time"],coupled["pos_out[2]"],color="C0",linestyle="dashed",label="position y (coupled)")
plt.plot(un_coupled["time"],un_coupled["pos_out[1]"],color="C1",linestyle="solid",label="position x (uncoupled)")
plt.plot(un_coupled["time"],un_coupled["pos_out[2]"],color="C1",linestyle="dashed",label="position y (uncoupled)")
# plt.plot(data["time"],data["pos_out[3]"],label="pos_out[3]")
plt.title("Position")
plt.ylabel("position [m]")
plt.xlabel("time [s]")
plt.legend()
plt.savefig("results/position.png")


plt.figure()
# plt.plot(data["time"],data["RPY_out[1]"],label="RPY_out[1]")
# plt.plot(data["time"],data["RPY_out[2]"],label="RPY_out[2]")
plt.plot(coupled["time"],coupled["RPY_out[3]"],label="rot angle z (coupled)")
plt.plot(un_coupled["time"],un_coupled["RPY_out[3]"],label="rot angle z (uncoupled)")
plt.legend()
plt.title("Orientation")
plt.ylabel("angle [rad]")
plt.xlabel("time [s]")
plt.savefig("results/orientation.png")


plt.figure()
# plt.plot(data["time"],data["t_in[1]"],label="t_in[1]")
# plt.plot(data["time"],data["t_in[2]"],label="t_in[2]")
plt.plot(coupled["time"],coupled["t_in[3]"],label="torque z (coupled)")
plt.plot(un_coupled["time"],un_coupled["t_in[3]"],label="torque z (uncoupled)")
plt.legend()
plt.title("Sloshing Torque")
plt.ylabel("Torque [Nm]")
plt.xlabel("time [s]")
plt.savefig("results/torque.png")


plt.figure()
plt.plot(coupled["time"],coupled["f_in[1]"],color="C0",linestyle="solid",label="force x (coupled)")
plt.plot(coupled["time"],coupled["f_in[2]"],color="C0",linestyle="dashed",label="force x (coupled)")
plt.plot(un_coupled["time"],un_coupled["f_in[1]"],color="C1",linestyle="solid",label="force x (uncoupled)")
plt.plot(un_coupled["time"],un_coupled["f_in[2]"],color="C1",linestyle="dashed",label="force x (uncoupled)")
# plt.plot(data["time"],data["f_in[3]"],label="f_in[3]")
plt.legend()
plt.title("Sloshing Force")
plt.ylabel("Force [N]")
plt.xlabel("time [s]")
plt.savefig("results/force.png")

plt.show()
