---
title: "FMU4FOAM"
keywords: sample homepage
sidebar: doc_sidebar
permalink: index.html
summary: FMU4FOAM with testsuite and documenation
---

# FMU4FOAM


### Install library OpenFOAM

```bash
pip install oftest conan
./build-ECI4FOAM.sh # init submodules
./Allwmake
```

### run example FMU


requires that OpenFOAM is build on that machine

```bash
cd example/TempControlFlange
./Allrun
```

```bash
python test-omsim.py > log
```


FMU4FOAM to quickly generate a library with testsuite and hosted documenation

zip or clone this library and edit it from there

## Installation OpenFOAM

```bash
pip install fmu4foam
```

## Installation Testsuite

install oftest to automatically test OpenFOAM with py.test

```
pip install oftest
```

run the test enviroment
```
py.test
```

{% include links.html %}

