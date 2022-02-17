---
title: 'FMU4FOAM: Coupling OpenFOAM with the Functional Mock-up Interface'
tags:
  - Python
  - OpenFOAM
  - FMI 
  - coupled simulation
authors:
  - name: Henning Scheufler^[corresponding author]
    orcid: 0000-0000-0000-0000
    affiliation: "1"
    email: Henning.Scheufler@dlr.de
  - name: Lars Ivar Hatledal
    orcid: 0000-0001-6436-7213
    affiliation: "2"
  - name: Lukas Opp
    orcid: 0000-0000-0000-0000
    affiliation: "1"
affiliations:
 - name: DLR Bremen, Institute for Space Systems
   index: 1
 - name: Norwegian University of Science and Technology
   index: 2
date: 7 Feburary 2022
bibliography: paper.bib
---

# Summary


Accurate CFD simulations often require complex boundary conditions that may be depend on numerous other engineering disciplines. Currently, these boundary conditions would require the addition of new models to the open-source CFD framework OpenFOAM which is error-prone and time-consuming. FMI standard offers a container (FMUs) to exchange multi-physics models that can be used to simulate these multi-physics. This library provides the possibility of coupled FMUs with OpenFOAM or exporting OpenFOAM as an FMU.


# Statement of need

Modeling experiments with CFD can be complex as boundary conditions in experimental setups are constrained by geometrical, physical, and mechanical aspects, just to name a few. To accurately predict experiments, these boundary conditions need to be modeled precisely, which may require implementing a new model in OpenFOAM that, for example, predicts the pressure drop and heat exchange in a pipe. The FMI standard addresses this issue by defining a container and interface to exchange multi-physical systems. The novel approach of FMU4FOAM enhances the modeling capabilities by combining the multi-physical domain models expressed by the FMI standard with OpenFOAM. This approach simplifies not only the modeling of complex experiments but also allows users to verify simplifying assumptions in system level models. For example, a CFD simulation can easily replace a passenger cabin model in a "Heating Venting and Air Conditioning"-System to compare both modeling approaches. The same approach is also helpful in Guidance and Control to verify transfer function.


# Acknowledgements

The authors gratefully acknowledge the financial support of "German Aerospace Center - DLR".

