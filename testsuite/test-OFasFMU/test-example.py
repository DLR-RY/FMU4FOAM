from pythonfmu.variables import Boolean
from FMU4FOAM import Fmi2Causality, Fmi2Variability, Real, Boolean , Integer, String
from FMU4FOAM import OF2Fmu


class Example(OF2Fmu):

    author = "John Doe"
    description = "A simple description"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.Qin = 0.0
        self.dTout = 0.0
        self.Tout = 298.0
        self.outputPath = "Testing" # will extract the OfCase files in Testing
        # However in this test it will be overwrite by pyfmi

        self.register_variable(Real("Qin", causality=Fmi2Causality.input))
        self.register_variable(Real("dTout", causality=Fmi2Causality.output))
        self.register_variable(Real("Tout", causality=Fmi2Causality.output))

        self.writeInterval = 1.0
        self.STDLAP = "Gauss linear corrected"
        self.STDGRAD = "pointCellsLeastSquares"
        

        self.register_variable(String("STDGRAD", causality=Fmi2Causality.parameter))
        self.register_variable(String("STDLAP", causality=Fmi2Causality.parameter))
        self.register_variable(Real("writeInterval", causality=Fmi2Causality.parameter))

