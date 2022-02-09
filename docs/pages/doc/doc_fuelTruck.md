---
title: Fuel Truck
permalink: doc_fuelTruck.html
keywords: OpenFOAM
sidebar: doc_sidebar
folder: doc
---

## Fuel Truck

This example is a modified version of the RollingWheelSetPulling from the Modelica standard library in the mechanics/multiBody module. As in the original model, the force is a function of time and pulls the fuel truck in the x-direction and after 3 seconds in the y-direction, resulting in a turning motion.

{% include image.html file="fuelTruck/FuelTruck.png"  alt="FuelTruck" caption="FuelTruck" %}

The FMU requires the sloshing forces and torque vector, and as FMI 2.0 does not support fields, they are indexed by `f_in[1-3]` and `t_in[1-3]`. OpenFOAM computes sloshing forces and torque with the position and the orientation vector (roll pitch yaw) represented by the variables `pos_out[1-3]` and `RPY_out[1-3]`. Sloshing torque also needs the center of rotation that is already given with `pos_out[1-3]`. The inputs and outputs in OpenFOAM are in the world coordinate system.

{% include image.html file="fuelTruck/fuelTruck_coupled.gif"  alt="results of the coupled simulation" caption="results of the coupled simulation" %}


{% include image.html file="fuelTruck/fuelTruck_uncoupled.gif"  alt="results of the uncoupled simulation" caption="results of the uncoupled simulation" %}

Comparing the uncoupled simulation and the coupled simulation shows that the fuel truck in the uncoupled simulation moves slightly faster as the fuel mass of roughly 250 kg is not considered. This is underlined by plotting the position over time. 

{% include image.html file="fuelTruck/position.png"   alt="position" caption="position" %}

As we also neglect the inertia of the fluid, the truck in the uncoupled simulation turns faster:

{% include image.html file="fuelTruck/orientation.png"   alt="orientation" caption="orientation" %}

The uncoupled simulation always predicts a sloshing force and torque of zero (see. FuelTruck_uncoupled.py) corresponding to massless fluid. 

{% include image.html file="fuelTruck/force.png"   alt="orientation" caption="force" %}
{% include image.html file="fuelTruck/torque.png"   alt="orientation" caption="torque" %}
