---
title: OpenFOAM
permalink: doc_OpenFOAM.html
keywords: OpenFOAM
sidebar: doc_sidebar
folder: doc
---

## OpenFOAM

OpenFOAM is a C++ library for solving PDEs with the special focus on Computional Fluid Dynamics with a large user base in academia and industry. With it incompressible, compressible, reacting and multiphase flow with numerous models can be simulated. These different flow types are all addressed by multiple [applications](https://www.openfoam.com/documentation/user-guide/a-reference/a.1-standard-solvers) that all have unique set of input parameters. The case consists of 3 main folder the "system", "constant" and the "time directories" as described in the [OpenFOAM user guide](https://www.openfoam.com/documentation/user-guide/2-openfoam-cases/2.1-file-structure-of-openfoam-cases). The "system" folder mainly contains the numerical parameters of a simulation, the "constant" folder the physical model such as the fluid properties or the selected turbulence model and the "time folder" store the computed fields.


{% include image.html file="OFCaseStructure.png"  alt="OFCaseStructure" caption="OpenFOAM case structure" %}

