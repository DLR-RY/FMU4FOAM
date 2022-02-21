---
title: 'FMU4FOAM: Coupling OpenFOAM with the Functional Mock-up Interface'
tags:
  - Python
  - OpenFOAM
  - FMI 
  - coupled simulation
authors:
  - name: Henning Scheufler^[corresponding author]
    orcid: 0000-0003-1578-9772
    affiliation: "1"
    email: Henning.Scheufler@dlr.de
  - name: Lars Ivar Hatledal
    orcid: 0000-0001-6436-7213
    affiliation: "2"
  - name: Lukas Opp
    orcid: 0000-0002-6791-4267
    affiliation: "1"
affiliations:
 - name: German Aerospace Center, Institute for Space Systems, Bremen
   index: 1
 - name: Department of ICT and Natural Sciences, Norwegian University of Science and Technology, Norway
   index: 2
date: 18 Feburary 2022
bibliography: paper.bib
---

# Summary


Accurate Computational Fluid Dynamics (CFD) simulations often require complex boundary conditions 
that may depend on numerous other engineering disciplines. 
Currently, these boundary conditions require new models to be modeled in and added to
the open-source CFD framework OpenFOAM, which is error-prone and time-consuming. 
The Functional Mock-up Interface (FMI) standard [@blockwitz2012functional] offers a 
standardised and tool-independent way of coupling models, which can be used to simulate the required multi-physics. 
This paper introduces FMU4FOAM, a library leveraging the FMI standard in order to provide the possibility 
to couple models written in other tools with OpenFOAM or even exporting OpenFOAM itself as an FMI compatible model.


# Statement of need

Modeling experiments with CFD can be complex as boundary conditions in experimental setups are constrained by geometrical, 
physical, and mechanical aspects, just to name a few [reference needed]. To accurately predict experiments, 
these boundary conditions need to be modeled precisely, which may require implementing a new model in OpenFOAM that, 
for example, predicts the pressure drop and heat exchange in a pipe. 
The FMI standard addresses this issue by defining tool-independent way of coupling multi-physical systems. 
The novel approach of FMU4FOAM enhances the modeling capabilities by combining the multi-physical domain models expressed by the FMI standard with OpenFOAM. 
This approach simplifies not only the modeling of complex experiments, but also allow users to verify simplifying assumptions in system level models. 
For example, a CFD simulation can easily replace a passenger cabin model in a "Heating Venting and Air Conditioning"-System to compare both modeling approaches. 
The same approach is also helpful in Guidance and Control to verify transfer function.


# Acknowledgements

The authors gratefully acknowledge the financial support of "German Aerospace Center - DLR".

# References


