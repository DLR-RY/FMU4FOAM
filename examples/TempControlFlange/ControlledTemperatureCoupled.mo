model ControlledTemperatureCoupled "Control temperature of a resistor"
  extends Modelica.Icons.Example;
  parameter Modelica.SIunits.Temperature TAmb(displayUnit="degC") = 293.15
    "Ambient Temperature";
  parameter Modelica.SIunits.TemperatureDifference TDif = 2
    "Error in Temperature";
  output Modelica.SIunits.Temperature TRes(displayUnit="degC") = heatingResistor.T_heatPort
    "Resulting Temperature";
  Modelica.Electrical.Analog.Basic.Ground ground
                              annotation (Placement(visible = true, transformation(extent = {{-96, -96}, {-76, -76}}, rotation = 0)));
  Modelica.Electrical.Analog.Sources.ConstantVoltage constantVoltage(V=0.05)
                                                        annotation (Placement(
        visible = true, transformation(origin = {-86, -46}, extent = {{-10, -10}, {10, 10}}, rotation = 270)));
  Modelica.Electrical.Analog.Basic.HeatingResistor heatingResistor(
    R_ref=10,
    T_ref=293.15,
    alpha=1/255, useHeatPort = true) annotation (Placement(visible = true, transformation(origin = {-20, -46}, extent = {{-10, 10}, {10, -10}}, rotation = 270)));
  Modelica.Electrical.Analog.Ideal.IdealOpeningSwitch idealSwitch
        annotation (Placement(visible = true, transformation(extent = {{-66, -46}, {-46, -26}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp(
    
    duration=10,height=25,
    offset= 298.15,
    startTime=2) annotation (Placement(visible = true, transformation(extent = {{44, 4}, {24, 24}}, rotation = 0)));
  Modelica.Blocks.Logical.OnOffController onOffController(bandwidth=TDif)
    annotation (Placement(visible = true, transformation(extent = {{4, -2}, {-16, 18}}, rotation = 0)));
  Modelica.Blocks.Logical.Not logicalNot
                                   annotation (Placement(visible = true, transformation(extent = {{-26, -2}, {-46, 18}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Components.GeneralTemperatureToHeatFlowAdaptor temperatureToHeatFlowAdaptor(use_pder = true)  annotation(
    Placement(visible = true, transformation(origin = {36, -46}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput dTin annotation(
    Placement(visible = true, transformation(origin = {78, -52}, extent = {{10, -10}, {-10, 10}}, rotation = 0), iconTransformation(extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput Tin annotation(
    Placement(visible = true, transformation(origin = {78, -38}, extent = {{10, -10}, {-10, 10}}, rotation = 0), iconTransformation(extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Sensors.TemperatureSensor temperatureSensor annotation(
    Placement(visible = true, transformation(origin = {20, -26}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Blocks.Interfaces.RealOutput Qout annotation(
    Placement(visible = true, transformation(origin = {68, -82}, extent = {{-10, -10}, {10, 10}}, rotation = 180), iconTransformation(origin = {74, -86}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(constantVoltage.n, heatingResistor.n) annotation(
    Line(points = {{-86, -56}, {-20, -56}}, color = {0, 0, 255}));
  connect(constantVoltage.n, ground.p) annotation(
    Line(points = {{-86, -56}, {-86, -76}}, color = {0, 0, 255}));
  connect(constantVoltage.p, idealSwitch.p) annotation(
    Line(points = {{-86, -36}, {-66, -36}}, color = {0, 0, 255}));
  connect(idealSwitch.n, heatingResistor.p) annotation(
    Line(points = {{-46, -36}, {-20, -36}}, color = {0, 0, 255}));
  connect(ramp.y, onOffController.reference) annotation(
    Line(points = {{23, 14}, {6, 14}}, color = {0, 0, 127}));
  connect(onOffController.y, logicalNot.u) annotation(
    Line(points = {{-17, 8}, {-24, 8}}, color = {255, 0, 255}));
  connect(logicalNot.y, idealSwitch.control) annotation(
    Line(points = {{-47, 8}, {-56, 8}, {-56, -29}}, color = {255, 0, 255}));
  connect(heatingResistor.heatPort, temperatureToHeatFlowAdaptor.heatPort) annotation(
    Line(points = {{-10, -46}, {34, -46}}, color = {191, 0, 0}));
  connect(dTin, temperatureToHeatFlowAdaptor.pder) annotation(
    Line(points = {{78, -52}, {44, -52}, {44, -41}, {39, -41}}, color = {0, 0, 127}));
  connect(temperatureToHeatFlowAdaptor.p, Tin) annotation(
    Line(points = {{39, -38}, {78, -38}}, color = {0, 0, 127}));
  connect(heatingResistor.heatPort, temperatureSensor.port) annotation(
    Line(points = {{-10, -46}, {20, -46}, {20, -36}}, color = {191, 0, 0}));
  connect(temperatureSensor.T, onOffController.u) annotation(
    Line(points = {{20, -16}, {20, 2}, {6, 2}}, color = {0, 0, 127}));
  connect(Qout, temperatureToHeatFlowAdaptor.f) annotation(
    Line(points = {{68, -82}, {42, -82}, {42, -54}, {40, -54}}, color = {0, 0, 127}));
  annotation (Documentation(info="<html>
<p>
A constant voltage of 10 V is applied to a
temperature dependent resistor of 10*(1+(T-20C)/(235+20C)) Ohms,
whose losses v**2/r are dissipated via a
thermal conductance of 0.1 W/K to ambient temperature 20 degree C.
The resistor is assumed to have a thermal capacity of 1 J/K,
having ambient temperature at the beginning of the experiment.
The temperature of this heating resistor is held by an OnOff-controller
at reference temperature within a given bandwidth +/- 1 K
by switching on and off the voltage source.
The reference temperature starts at 25 degree C
and rises between t = 2 and 8 seconds linear to 50 degree C.
An appropriate simulating time would be 10 seconds.
</p>
</html>"),        experiment(StopTime=10, Interval=0.001));
end ControlledTemperatureCoupled;