from pythonfmu.variables import Boolean
from FMU4FOAM import Fmi2Causality, Fmi2Variability, Real, Boolean , Integer, String
from FMU4FOAM import OF2Fmu


class Example(OF2Fmu):

    author = "John Doe"
    description = "A simple description"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.r_in = 1.0
        self.r_out = 2.0
        self.i_in = 1
        self.i_out = 2
        self.b_in = True
        self.b_out = False

        self.s_in = "in"
        self.s_out = "out"
        
        self.register_variable(Real("r_in", causality=Fmi2Causality.input))
        self.register_variable(Real("r_out", causality=Fmi2Causality.output))

        self.register_variable(Boolean("b_in", causality=Fmi2Causality.input))
        self.register_variable(Boolean("b_out", causality=Fmi2Causality.output))

        self.register_variable(Integer("i_in", causality=Fmi2Causality.input))
        self.register_variable(Integer("i_out", causality=Fmi2Causality.output))

        self.register_variable(String("s_in", causality=Fmi2Causality.input))
        self.register_variable(String("s_out", causality=Fmi2Causality.output))

