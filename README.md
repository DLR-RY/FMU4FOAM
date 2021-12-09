# FMU4FOAM

> A lightweight framework that enables the packaging of Python 3 code or CSV files as co-simulation FMUs (following FMI version 2.0).

### Documentation


[Documentation](https://DLR-RY.github.io/FMU4FOAM/)



### How to install?



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

