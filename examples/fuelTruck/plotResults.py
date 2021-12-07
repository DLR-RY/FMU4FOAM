import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("results.csv")

colval = data.columns.values.copy()
for idx,col in enumerate(colval):
    col = col.replace('model.root.system1.',"")
    col = col.replace('"',"")
    colval[idx] = col
data.columns = colval
print(data)
data = data[data["time"] > 0.01]

plt.plot(data["time"],data["pos_out[1]"],label="pos_out[1]")
plt.plot(data["time"],data["pos_out[2]"],label="pos_out[2]")
# plt.plot(data["time"],data["pos_out[3]"],label="pos_out[3]")
plt.title("position")
plt.ylabel("position [m]")
plt.xlabel("time [s]")
plt.savefig("results/position.png")
plt.legend()


plt.figure()
# plt.plot(data["time"],data["RPY_out[1]"],label="RPY_out[1]")
# plt.plot(data["time"],data["RPY_out[2]"],label="RPY_out[2]")
plt.plot(data["time"],data["RPY_out[3]"],label="RPY_out[3]")
plt.legend()
plt.title("orientation")
plt.ylabel("angle [rad]")
plt.xlabel("time [s]")
plt.savefig("results/orientation.png")


plt.figure()
# plt.plot(data["time"],data["t_in[1]"],label="t_in[1]")
# plt.plot(data["time"],data["t_in[2]"],label="t_in[2]")
plt.plot(data["time"],data["t_in[3]"],label="t_in[3]")
plt.legend()
plt.title("sloshing torque")
plt.ylabel("torque [Nm]")
plt.xlabel("time [s]")
plt.savefig("results/torque.png")


plt.figure()
plt.plot(data["time"],data["f_in[1]"],label="f_in[1]")
plt.plot(data["time"],data["f_in[2]"],label="f_in[2]")
# plt.plot(data["time"],data["f_in[3]"],label="f_in[3]")
plt.legend()
plt.title("sloshing force")
plt.ylabel("torque [N]")
plt.xlabel("time [s]")
plt.savefig("results/force.png")

plt.show()
