# FMU4FOAM

> A framework that enables the coupling of OpenFOAM with FMUs or the distribution of OpenFOAM as an FMU


Accurate CFD simulations often require complex boundary conditions depending on numerous engineering disciplines. Currently, these boundary conditions would require the addition of new models to the open-source CFD framework OpenFOAM which is error-prone and time-consuming. FMI standard offers a container (FMUs) to exchange multi-physics models that can be used to simulate these multi-physics. This library provides the possibility of coupled FMUs with OpenFOAM or exporting OpenFOAM as an FMU.

### Documentation


[Documentation](https://DLR-RY.github.io/FMU4FOAM/) hosted in github pages


### How to install?

Assumes that the [OpenFOAM version 2012](https://www.openfoam.com/download/release-history) or higher and python v3.8+ is already installed

1. Install `FMU4FOAM` package:

requires conan and pythonfmu
```bash
pip install fmu4foam
```
### Compile OpenFOAM

```bash
./build-ECI4FOAM.sh # init submodule
./Allwmake
```

more details are found in the [documentation](https://DLR-RY.github.io/FMU4FOAM/)

### run example


```bash
cd examples/TempControlFlange/
./Allrun
```

