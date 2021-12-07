model HVACSystem "simple HVAC System"

  parameter Modelica.Thermal.FluidHeatFlow.Media.Medium medium=Modelica.Thermal.FluidHeatFlow.Media.Air_30degC()
    "Cooling medium"
    annotation(choicesAllMatching=true);
  parameter Modelica.SIunits.Temperature TAmb(displayUnit="degC")=293.15
    "Ambient temperature";

  Modelica.Thermal.FluidHeatFlow.Sources.Ambient ambient1(
    constantAmbientPressure(displayUnit = "Pa") = 0,constantAmbientTemperature=TAmb, medium=medium, usePressureInput = false, useTemperatureInput = true)
    annotation (Placement(visible = true, transformation(extent = {{-90, 20}, {-110, 40}}, rotation = 0)));
  Modelica.Thermal.FluidHeatFlow.Sources.VolumeFlow pump(
    medium=medium,
    m=0,
    T0=TAmb,
    useVolumeFlowInput=true,
    constantVolumeFlow=1)
    annotation (Placement(visible = true, transformation(extent = {{-40, 20}, {-20, 40}}, rotation = 0)));
  Modelica.Thermal.FluidHeatFlow.Components.Pipe pipe(
    
    T0=TAmb,
    T0fixed=true,
    V_flowLaminar=0.1,
    V_flowNominal=1,
    dpLaminar(displayUnit="Pa") = 0.1,
    dpNominal(displayUnit="Pa") = 1,
    h_g=0,
    m= 0.1,medium=medium,
    useHeatPort=true)
    annotation (Placement(visible = true, transformation(extent = {{0, 20}, {20, 40}}, rotation = 0)));
  Modelica.Thermal.FluidHeatFlow.Sources.Ambient ambient2(constantAmbientTemperature=TAmb, medium=medium,
    constantAmbientPressure=0)
    annotation (Placement(visible = true, transformation(extent = {{70, 20}, {90, 40}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Components.HeatCapacitor heatCapacitor(
                   C= 400, T( fixed=true,start=TAmb))
    annotation (Placement(visible = true, transformation(origin = {40, -60}, extent = {{-10, 10}, {10, -10}}, rotation = 90)));
  Modelica.Thermal.HeatTransfer.Components.Convection convection
    annotation (Placement(visible = true, transformation(origin = {10, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Blocks.Sources.Constant thermalConductance(k= 50000)
    annotation (Placement(visible = true, transformation(extent = {{-32, -10}, {-12, 10}}, rotation = 0)));
  Modelica.Thermal.FluidHeatFlow.Sensors.TemperatureSensor temperatureSensor(medium = medium)  annotation(
    Placement(visible = true, transformation(origin = {30, 76}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Thermal.FluidHeatFlow.Sensors.MassFlowSensor massFlowSensor(medium = medium) annotation(
    Placement(visible = true, transformation(origin = {48, 30}, extent = {{-10, 10}, {10, -10}}, rotation = 0)));
  Modelica.Electrical.Analog.Sources.ConstantVoltage constantVoltage(V = 500) annotation(
    Placement(visible = true, transformation(origin = {-90, -60}, extent = {{-10, -10}, {10, 10}}, rotation = 270)));
  Modelica.Electrical.Analog.Basic.HeatingResistor heatingResistor(R_ref = 1, T_ref = 293.15, alpha = 1 / 255, useHeatPort = true) annotation(
    Placement(visible = true, transformation(origin = {-30, -60}, extent = {{-10, 10}, {10, -10}}, rotation = 270)));
  Modelica.Electrical.Analog.Basic.Ground ground annotation(
    Placement(visible = true, transformation(extent = {{-100, -98}, {-80, -78}}, rotation = 0)));
  Modelica.Blocks.Continuous.LimPID pid( Td = 0, Ti = 10,initType = Modelica.Blocks.Types.InitPID.InitialState, k = 10, limitsAtInit = true, xd_start = 0, xi_start = 0, yMax = 1000, yMin = 0.0001, y_start = 10000)  annotation(
    Placement(visible = true, transformation(origin = {-84, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Electrical.Analog.Basic.VariableConductor conductor annotation(
    Placement(visible = true, transformation(origin = {-64, -50}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealOutput Tout annotation(
    Placement(visible = true, transformation(origin = {76, 76}, extent = {{-10, -10}, {10, 10}}, rotation = 0), iconTransformation(origin = {72, 78}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealOutput mout annotation(
    Placement(visible = true, transformation(origin = {76, 60}, extent = {{-10, -10}, {10, 10}}, rotation = 0), iconTransformation(origin = {72, 78}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Sensors.HeatFlowSensor heatFlowSensor annotation(
    Placement(visible = true, transformation(origin = {8, -60}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Continuous.LimPID limPID(Td = 0, Ti = 10, initType = Modelica.Blocks.Types.InitPID.InitialState, k = 1, limitsAtInit = true, xd_start = 0, xi_start = 0, yMax = 10, yMin = 0.00001, y_start = 10000) annotation(
    Placement(visible = true, transformation(origin = {-76, 84}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput Tin annotation(
    Placement(visible = true, transformation(origin = {-150, 50}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-176, 46}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput Tinlet annotation(
    Placement(visible = true, transformation(origin = {-168, 24}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-176, 46}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
  Modelica.Blocks.Sources.Ramp ramp(duration = 150, height = 30, offset = 298, startTime = 50)  annotation(
    Placement(visible = true, transformation(origin = {-174, 84}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(ambient1.flowPort, pump.flowPort_a) annotation(
    Line(points = {{-90, 30}, {-40, 30}}, color = {255, 0, 0}));
  connect(pump.flowPort_b, pipe.flowPort_a) annotation(
    Line(points = {{-20, 30}, {0, 30}}, color = {255, 0, 0}));
  connect(convection.solid, heatCapacitor.port) annotation(
    Line(points = {{10, -10}, {10, -35}, {30, -35}, {30, -60}}, color = {191, 0, 0}));
  connect(pipe.heatPort, convection.fluid) annotation(
    Line(points = {{10, 20}, {10, 10}}, color = {191, 0, 0}));
  connect(thermalConductance.y, convection.Gc) annotation(
    Line(points = {{-11, 0}, {0, 0}}, color = {0, 0, 127}));
  connect(pipe.flowPort_b, temperatureSensor.flowPort) annotation(
    Line(points = {{20, 30}, {20, 76}}, color = {255, 0, 0}));
  connect(pipe.flowPort_b, massFlowSensor.flowPort_a) annotation(
    Line(points = {{20, 30}, {38, 30}}, color = {255, 0, 0}));
  connect(massFlowSensor.flowPort_b, ambient2.flowPort) annotation(
    Line(points = {{58, 30}, {70, 30}}, color = {255, 0, 0}));
  connect(constantVoltage.n, ground.p) annotation(
    Line(points = {{-90, -70}, {-90, -78}}, color = {0, 0, 255}));
  connect(constantVoltage.n, heatingResistor.n) annotation(
    Line(points = {{-90, -70}, {-30, -70}}, color = {0, 0, 255}));
  connect(constantVoltage.p, conductor.p) annotation(
    Line(points = {{-90, -50}, {-74, -50}}, color = {0, 0, 255}));
  connect(conductor.n, heatingResistor.p) annotation(
    Line(points = {{-54, -50}, {-30, -50}}, color = {0, 0, 255}));
  connect(pid.y, conductor.G) annotation(
    Line(points = {{-72, 0}, {-64, 0}, {-64, -38}}, color = {0, 0, 127}));
  connect(temperatureSensor.y, Tout) annotation(
    Line(points = {{42, 76}, {76, 76}}, color = {0, 0, 127}));
  connect(massFlowSensor.y, mout) annotation(
    Line(points = {{48, 42}, {48, 60}, {76, 60}}, color = {0, 0, 127}));
  connect(heatingResistor.heatPort, heatFlowSensor.port_a) annotation(
    Line(points = {{-20, -60}, {-2, -60}}, color = {191, 0, 0}));
  connect(heatFlowSensor.port_b, heatCapacitor.port) annotation(
    Line(points = {{18, -60}, {30, -60}}, color = {191, 0, 0}));
  connect(limPID.y, pump.volumeFlow) annotation(
    Line(points = {{-64, 84}, {-30, 84}, {-30, 40}}, color = {0, 0, 127}));
  connect(Tin, limPID.u_m) annotation(
    Line(points = {{-150, 50}, {-76, 50}, {-76, 72}}, color = {0, 0, 127}));
  connect(Tinlet, ambient1.ambientTemperature) annotation(
    Line(points = {{-168, 24}, {-110, 24}}, color = {0, 0, 127}));
  connect(ramp.y, limPID.u_s) annotation(
    Line(points = {{-163, 84}, {-88, 84}}, color = {0, 0, 127}));
  connect(ramp.y, pid.u_s) annotation(
    Line(points = {{-162, 84}, {-134, 84}, {-134, 0}, {-96, 0}}, color = {0, 0, 127}));
  connect(Tin, pid.u_m) annotation(
    Line(points = {{-150, 50}, {-134, 50}, {-134, -24}, {-84, -24}, {-84, -12}}, color = {0, 0, 127}));
  annotation(
    Documentation(info = "<html>
<p>
1st test example: SimpleCooling
</p>
A prescribed heat source dissipates its heat through a thermal conductor to a coolant flow. The coolant flow is taken from an ambient and driven by a pump with prescribed mass flow.<br>
<strong>Results</strong>:<br>
<table>
<tr>
<td><strong>output</strong></td>
<td><strong>explanation</strong></td>
<td><strong>formula</strong></td>
<td><strong>actual steady-state value</strong></td>
</tr>
<tr>
<td>dTSource</td>
<td>Source over Ambient</td>
<td>dtCoolant + dtToPipe</td>
<td>20 K</td>
</tr>
<tr>
<td>dTtoPipe</td>
<td>Source over Coolant</td>
<td>Losses / ThermalConductor.G</td>
<td>10 K</td>
</tr>
<tr>
<td>dTCoolant</td>
<td>Coolant's temperature increase</td>
<td>Losses * cp * massFlow</td>
<td>10 K</td>
</tr>
</table>
</html>"),
    experiment(StopTime = 1.0, Interval = 0.001),
    uses(Modelica(version = "3.2.3")));
end HVACSystem;