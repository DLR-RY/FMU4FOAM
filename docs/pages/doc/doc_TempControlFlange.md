---
title: Temperature Control in a Flange
permalink: doc_TempControlFlange.html
keywords: OpenFOAM
sidebar: doc_sidebar
folder: doc
---

## Temperature Control in a Flange

This example demonstrates the ability to pass boundary condition data using a pregenerated FMU into the OpenFoam environment using the FMU4FOAM extension. The FMU was generated using OpenModelica and is very similar to Modelica Standard Library's "ControlledTemperature" example from the HeatTransfer library shown in the following figure.

{% include image.html file="TempControlFlange/original_schematic.png"  alt="Schematic of the ControlledTemperature example in OpenModelica" caption="Schematic of the ControlledTemperature example in OpenModelica" %}

In this FMU4FOAM example case, the FMU is used to control the temperature of a flange like geometry by imposing a heat flux boundary condition to the OpenFoam domain in order to heat up the flange. 

The schematic of the FMU is depicted in the following sketch:

{% include image.html file="TempControlFlange/ControlledTemperatureCoupled.png"  alt="Schematic of the FMU in OpenModelica" caption="Schematic of the FMU in OpenModelica" %}

There are two input variables to the FMU:

1. The actual temperature of the patch, the boundary condition is assigned to
2. the time derivative of the temperature at that specific patch

The temperature is then compared to a target temperature at the current time step by an on/off controller element. If the temperature is lower than the target temperature by a predefined threshold, the temperature controller triggers a switch thus activating a heater element. The output signal of that heater element is a heat flux signal that is then used as the output signal of the FMU dictating the heat flux at the boundary in the OpenFoam domain.

In this particular case, the target temperature is a rising linear function. The temperature controller is responible for applying the necessary heat flux to heat up the flange. The heat flux output is shown in the following figure.

{% include image.html file="TempControlFlange/Q.png"  alt="Output heat flux Q" caption="Output heat flux Q" %}

By applying the heat flux from the previous plot to the patch of the OpenFoam domain, the temperature of the flange follows the target function in a saw tooth pattern as expected using an on/off controller with build-in hysteresis. 

{% include image.html file="TempControlFlange/T.png"  alt="Input and target temperature" caption="Input and target temperature" %}

An animation of the surface temperature is shown here:

{% include image.html file="TempControlFlange/TemperatureControlledFlange.gif"  alt="Input and target temperature" caption="Input and target temperature" %}

