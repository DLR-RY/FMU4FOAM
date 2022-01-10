---
title: FMUs in OpenFOAM
permalink: doc_FMUinOF.html
keywords: design
sidebar: doc_sidebar
folder: doc
---

This libray offers the opportunity to connect OpenFOAM with FMUs in two ways

- generate an FMU from an OpenFOAM case
- run FMUs from OpenFOAM 




## run FMUs within an OpenFOAM solver

One of the biggest feature of OpenFOAM is that is can be easily be extended by linking new library to the solver at runtime. One example are *functionObjects* that are loaded at runtime and have to be placed in the *system/controlDict* in the functions dictionary. To run FMUs in OpenFOAM, the *FMUSimulator* needs to be copied into the *controlDict*:

```cpp

libs(externalComm);

functions
{
    FMUSimulator
    {
        type            FMUSimulator; // execute functionObjects FMUSimualtor
        libs            (pyFMUSim); // load "libpyFMUSim.so"
        pyClassName     controlledTemperature; // execute python class controlledTemperature
        pyFileName      controlledTemperature; // load file controlledTemperature.py
    }
}
```

With this entry, the OpenFOAM-solver loads the library *pyFMUSIM* - short for *libpyFMUSIM.so* - that specifies a new implementation of the functionObject *FMUSimulator*. Underthehood, *FMUSimulator* embeds a python interpreter that manages the FMUs. The embedded python interpreter simplifies the installation through *pip* or *conda* and the user can chose from multiple frameworks such as *pyfmi*, *fmpy* or *omsimulator*. Another huge advantage is that the user can connect multiple fmus in python and test them before connecting them to OpenFOAM. After sucessfully testing the FMU System, the *FMUSimulator* will call the class *pyClassName* in *pyFileName* that assumed to be located in the root of the OpenFOAM case. The class looks as followed:

```python
import FMU4FOAM
from OMSimulator import OMSimulator


class controlledTemperature(FMU4FOAM.FMUBase):

    def __init__(self,endTime,filename):
        self.oms = OMSimulator()
        super().__init__(filename,self.oms)
        self.oms.setTempDirectory("./temp/")
        self.oms.newModel("model")
        self.oms.addSystem("model.root", self.oms.system_wc)

        self.oms.addSubModel("model.root.system1", "ControlledTemperatureCoupled.fmu")

        # simulation settings
        self.oms.setResultFile("model", "controlledTemperature.csv")
        self.oms.setStopTime("model", endTime)
        # self.oms.setFixedStepSize("model.root", 1e-7)

        # instantiate FMUs
        self.oms.instantiate("model")
        self.oms.setReal("model.root.system1.Tin", 298)
        self.oms.setReal("model.root.system1.dTin", 0)

        self.oms.initialize("model")

    def stepUntil(self,t):
        self.oms.stepUntil("model",t)

    def __del__(self):
        self.oms.terminate("model")
        self.oms.delete("model")
```

It consists of a constructor that the initilzes the simulation, a destructor that finalizes the simulation and a method that advances the FMU system in time called stepUntil. The data transfer between OpenFOAM and the FMUs is handled by FMUBase with the method *from_OF* and *to_OF*:


```python
    class FMUBase(ABC):

     ...

    def from_OF(self,dumped_input_json : str):
        ext_inputs = json.loads(dumped_input_json)
        for k in self.coupleData.from_OF:
            var = (k,ext_inputs[k])
            self.coupleData.setValue(self.oms,var)

    def to_OF(self) -> str:
        d = {}
        for k in self.coupleData.to_OF:
            d[k] = self.coupleData.getValue(self.oms,k)
        return str(json.dumps(d))
```

It is possible to overload this function and replace it by custom solution that fits your needs but in most cases it should be sufficient to specify the FMU.json in the root of the OpenFOAM case:

```json
{
    "mapping": [["Tin","model.root.system1.Tin"],
                ["dTin","model.root.system1.dTin"],
                ["Qout","model.root.system1.Qout"]],
    "from_OF": {"Tin":"REAL",
                "dTin":"REAL"},
    "to_OF":   {"Qout":"REAL"}
}
```

The FMU.json has to have 3 entries:

- mapping : List of all OpenFOAM variables ----> FMU variables
- from_OF : List of the variables and its types computed by OpenFOAM
- to_OF : List of the variables and its types computed by FMU

NOTE: from_OF and to_OF the OpenFOAM variables names need to be specified and the mapping assumes OF to FMU
