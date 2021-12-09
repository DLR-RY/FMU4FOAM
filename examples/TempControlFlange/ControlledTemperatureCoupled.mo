model ControlledTemperatureCoupled "Control temperature of a resistor"
  extends Modelica.Icons.Example;
  parameter Modelica.SIunits.Temperature TAmb(displayUnit="degC") = 293.15
    "Ambient Temperature";
  parameter Modelica.SIunits.TemperatureDifference TDif = 2
    "Error in Temperature";
  output Modelica.SIunits.Temperature TRes(displayUnit="degC") = heatingResistor.T_heatPort
    "Resulting Temperature";
  Modelica.Electrical.Analog.Basic.Ground ground
                              annotation (Placement(transformation(extent={
            {-100,-100},{-80,-80}})));
  Modelica.Electrical.Analog.Sources.ConstantVoltage constantVoltage(V=0.05)
                                                        annotation (Placement(
        transformation(
        origin={-90,-50},
        extent={{-10,-10},{10,10}},
        rotation=270)));
  Modelica.Electrical.Analog.Basic.HeatingResistor heatingResistor(
    R_ref=10,
    T_ref=293.15,
    alpha=1/255, useHeatPort = true) annotation (Placement(visible = true, transformation(origin = {-24, -50}, extent = {{-10, 10}, {10, -10}}, rotation = 270)));
  Modelica.Electrical.Analog.Ideal.IdealOpeningSwitch idealSwitch
        annotation (Placement(transformation(extent={{-70,-50},{-50,-30}})));
  Modelica.Blocks.Sources.Ramp ramp(
    
    duration=10,height=25,
    offset= 298.15,
    startTime=2) annotation (Placement(visible = true, transformation(extent = {{40, 0}, {20, 20}}, rotation = 0)));
  Modelica.Blocks.Logical.OnOffController onOffController(bandwidth=TDif)
    annotation (Placement(visible = true, transformation(extent = {{0, -6}, {-20, 14}}, rotation = 0)));
  Modelica.Blocks.Logical.Not logicalNot
                                   annotation (Placement(visible = true, transformation(extent = {{-30, -6}, {-50, 14}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Components.GeneralTemperatureToHeatFlowAdaptor temperatureToHeatFlowAdaptor(use_pder = true)  annotation(
    Placement(visible = true, transformation(origin = {32, -50}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput dTin annotation(
    Placement(visible = true, transformation(origin = {74, -56}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput Tin annotation(
    Placement(visible = true, transformation(origin = {74, -42}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Sensors.TemperatureSensor temperatureSensor annotation(
    Placement(visible = true, transformation(origin = {16, -30}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Blocks.Interfaces.RealOutput Qout annotation(
    Placement(visible = true, transformation(origin = {64, -86}, extent = {{-10, -10}, {10, 10}}, rotation = 180), iconTransformation(origin = {74, -86}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(constantVoltage.n, heatingResistor.n) annotation(
    Line(points = {{-90, -60}, {-24, -60}}, color = {0, 0, 255}));
  connect(constantVoltage.n, ground.p) annotation(
    Line(points = {{-90, -60}, {-90, -80}}, color = {0, 0, 255}));
  connect(constantVoltage.p, idealSwitch.p) annotation(
    Line(points = {{-90, -40}, {-70, -40}}, color = {0, 0, 255}));
  connect(idealSwitch.n, heatingResistor.p) annotation(
    Line(points = {{-50, -40}, {-24, -40}}, color = {0, 0, 255}));
  connect(ramp.y, onOffController.reference) annotation(
    Line(points = {{19, 10}, {2, 10}}, color = {0, 0, 127}));
  connect(onOffController.y, logicalNot.u) annotation(
    Line(points = {{-21, 4}, {-28, 4}}, color = {255, 0, 255}));
  connect(logicalNot.y, idealSwitch.control) annotation(
    Line(points = {{-51, 4}, {-60, 4}, {-60, -33}}, color = {255, 0, 255}));
  connect(heatingResistor.heatPort, temperatureToHeatFlowAdaptor.heatPort) annotation(
    Line(points = {{-14, -50}, {30, -50}}, color = {191, 0, 0}));
  connect(dTin, temperatureToHeatFlowAdaptor.pder) annotation(
    Line(points = {{74, -56}, {40, -56}, {40, -45}, {35, -45}}, color = {0, 0, 127}));
  connect(temperatureToHeatFlowAdaptor.p, Tin) annotation(
    Line(points = {{35, -42}, {74, -42}}, color = {0, 0, 127}));
  connect(heatingResistor.heatPort, temperatureSensor.port) annotation(
    Line(points = {{-14, -50}, {16, -50}, {16, -40}}, color = {191, 0, 0}));
  connect(temperatureSensor.T, onOffController.u) annotation(
    Line(points = {{16, -20}, {16, -2}, {2, -2}}, color = {0, 0, 127}));
  connect(Qout, temperatureToHeatFlowAdaptor.f) annotation(
    Line(points = {{64, -86}, {38, -86}, {38, -58}, {36, -58}}, color = {0, 0, 127}));
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