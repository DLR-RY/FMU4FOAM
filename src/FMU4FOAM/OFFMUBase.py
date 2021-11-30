from abc import ABC, abstractmethod
import json

class CouplingData:

    def __init__(self,filename) -> None:
        with open(filename) as file:
            d = json.load(file)
        # maps from OF to FMU vice versa
        self.OF_to_FMU = {k:v for k,v in d["mapping"]} 
        self.FMU_to_OF = {v:k for k,v in d["mapping"]}

        self.from_OF = d["from_OF"] # FMU inputs
        self.to_OF = d["to_OF"] # FMU ouputs

    def setValue(self,oms,var) -> None:
        of_key, val = var
        key = self.OF_to_FMU[of_key]
        var_type = self.from_OF[of_key]
        if var_type == "REAL":
            oms.setReal(key,val)

    def getValue(self,oms,of_key):
        key = self.OF_to_FMU[of_key]
        var_type = self.to_OF[of_key]
        if var_type == "REAL":
            return oms.getReal(key)[0]

class FMUBase(ABC):

    @abstractmethod
    def __init__(self,filename,oms):
        self.coupleData = CouplingData(filename)
        self.oms = oms


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

    @abstractmethod
    def stepUntil(self,t):
        pass

    @abstractmethod
    def __del__(self):
        pass