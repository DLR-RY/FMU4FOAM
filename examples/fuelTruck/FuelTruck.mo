model FuelTruck "Rolling wheel set that is pulled by a force"
   extends Modelica.Icons.Example;

  Modelica.Mechanics.MultiBody.Forces.WorldForce force(animation=false)
    annotation (Placement(visible = true, transformation(extent = {{20, 30}, {40, 50}}, rotation = 0)));
  inner Modelica.Mechanics.MultiBody.World world(
    label2="z",
    n={0,0,-1},
    animateGround=true,
    groundLength_u=3,
    groundColor={130,200,130})
    annotation (Placement(transformation(extent={{-80,-60},{-60,-40}})));
  Modelica.Mechanics.MultiBody.Parts.RollingWheelSet wheelSet(
    
    der_theta1(fixed=true),
    der_theta2(fixed=true),
    phi(displayUnit = "rad", fixed = true),
    theta1(displayUnit = "rad", fixed = true),
    theta2(displayUnit = "rad", fixed = true),
    wheelDistance=0.5,
    wheelMass=250,wheelRadius=0.1,
    wheel_I_axis= 5,
    wheel_I_long= 10,
    x( fixed=true,start= 0.0),
    y( fixed=true,start= 0.0))
    annotation (Placement(transformation(extent={{-20,-10},{0,10}})));
  Modelica.Mechanics.MultiBody.Parts.Body body(m=0.01, r_CM={0,0,0},
    animation=false)
    annotation (Placement(transformation(extent={{60,-10},{80,10}})));
  Modelica.Blocks.Sources.CombiTimeTable combiTimeTable(table = [0, 4000, 0, 0; 1, 4000, 0, 0; 2, 4000, 0, 0; 4, 0, 4000, 0; 5, 0, 4000, 0], tableOnFile = false)
    annotation (Placement(transformation(extent={{-20,30},{0,50}})));
  Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(
                       
    animation=true,r={1,0,0},
    width=0.04)
    annotation (Placement(transformation(extent={{20,-10},{40,10}})));
  Modelica.Mechanics.MultiBody.Visualizers.FixedShape shape(
    final lengthDirection={0,1,0},
    final widthDirection={1,0,0},
    final shapeType="pipe",
    final r_shape={0,-wheelSet.wheelWidth,0},
    final length=2*wheelSet.wheelWidth,
    final width=2*wheelSet.wheelRadius,
    final height=2*wheelSet.wheelRadius,
    final color={0,128,255},
    final extra=0.8) annotation (Placement(transformation(extent={{-10,-10},
            {10,10}},
        origin={70,30})));
  Modelica.Mechanics.MultiBody.Sensors.AbsoluteSensor absoluteSensor(animation = false,get_angles = true, get_r = true, resolveInFrame = Modelica.Mechanics.MultiBody.Types.ResolveInFrameA.world)  annotation(
    Placement(visible = true, transformation(origin = {20, -28}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealOutput[3] pos_out annotation(
    Placement(visible = true, transformation(origin = {-30, -84}, extent = {{-10, -10}, {10, 10}}, rotation = 0), iconTransformation(origin = {44, -82}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealOutput[3] RPY_out annotation(
    Placement(visible = true, transformation(origin = {48, -80}, extent = {{-10, -10}, {10, 10}}, rotation = 0), iconTransformation(origin = {8, -82}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.WorldForceAndTorque Forces_Sloshing(resolveInFrame = Modelica.Mechanics.MultiBody.Types.ResolveInFrameB.world)  annotation(
    Placement(visible = true, transformation(origin = {16, 78}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput[3] f_in annotation(
    Placement(visible = true, transformation(origin = {-54, 72}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-56, 82}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
  Modelica.Blocks.Interfaces.RealInput[3] t_in annotation(
    Placement(visible = true, transformation(origin = {-48, 84}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-56, 82}, extent = {{-20, -20}, {20, 20}}, rotation = 0)));
equation
  connect(combiTimeTable.y, force.force) annotation(
    Line(points = {{1, 40}, {18, 40}}, color = {0, 0, 127}));
  connect(fixedTranslation.frame_a, wheelSet.frameMiddle) annotation(
    Line(points = {{20, 0}, {10, 0}, {10, -4}, {-10, -4}, {-10, -2}}, color = {95, 95, 95}, thickness = 0.5));
  connect(fixedTranslation.frame_b, body.frame_a) annotation(
    Line(points = {{40, 0}, {60, 0}}, color = {95, 95, 95}, thickness = 0.5));
  connect(force.frame_b, fixedTranslation.frame_b) annotation(
    Line(points = {{40, 40}, {40, 0}}, color = {95, 95, 95}, thickness = 0.5));
  connect(shape.frame_a, fixedTranslation.frame_b) annotation(
    Line(points = {{60, 30}, {50, 30}, {50, 0}, {40, 0}}, color = {95, 95, 95}, thickness = 0.5));
  connect(wheelSet.frameMiddle, absoluteSensor.frame_a) annotation(
    Line(points = {{-10, -2}, {-10, -28}, {10, -28}}, color = {95, 95, 95}));
  connect(absoluteSensor.r, pos_out) annotation(
    Line(points = {{10, -38}, {-30, -38}, {-30, -84}}, color = {0, 0, 127}, thickness = 0.5));
  connect(absoluteSensor.angles, RPY_out) annotation(
    Line(points = {{22, -38}, {22, -80}, {48, -80}}, color = {0, 0, 127}, thickness = 0.5));
  connect(f_in, Forces_Sloshing.force) annotation(
    Line(points = {{-54, 72}, {4, 72}}, color = {0, 0, 127}, thickness = 0.5));
  connect(t_in, Forces_Sloshing.torque) annotation(
    Line(points = {{-48, 84}, {4, 84}}, color = {0, 0, 127}, thickness = 0.5));
  connect(Forces_Sloshing.frame_b, fixedTranslation.frame_b) annotation(
    Line(points = {{26, 78}, {48, 78}, {48, 0}, {40, 0}}));
  annotation(
    experiment(StopTime = 3),
    Documentation(info = "<html>
<p>
Demonstrates how a RollingWheelSet (two wheels rigidly coupled together) is rolling
on ground when pulled by an external force..
</p>
</html>"));
end FuelTruck;