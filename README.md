# FMU4FOAM

> A lightweight framework that enables the packaging of Python 3 code or CSV files as co-simulation FMUs (following FMI version 2.0).


### How do I build an FMU from python code?

1. Install `FMU4FOAM` package:

requires conan and pythonfmu
```bash
pip install .
```
### Compile OpenFOAM

```bash
./build-ECI4FOAM.sh # init submodule
./Allwmake
```


### Create FMU


```bash
cd test
fmu4foam build -f test-example.py -of damBreak/ --no-external-tool
```

```bash
python test-omsim.py > log
```
