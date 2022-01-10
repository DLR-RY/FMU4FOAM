---
title: Install Jekyll on Windows
permalink: doc_installation.html
keywords: jekyll on windows, pc, ruby, ruby dev kit
sidebar: doc_sidebar
folder: doc
---

## Prerequisite

The library requires OpenFOAM version v2012 or higher. OpenFOAM can be build from [source](https://develop.openfoam.com/Development/openfoam/-/blob/master/doc/Build.md) or installed with the [package manager](https://develop.openfoam.com/Development/openfoam/-/wikis/precompiled). The installation also requires the installation of python on the system that can easily be installed with the [anaconda](https://www.anaconda.com/products/individual) 

### Linux

```bash
    curl -s https://dl.openfoam.com/add-debian-repo.sh | sudo bash
    wget -q -O - https://dl.openfoam.com/add-debian-repo.sh | sudo bash
    sudo apt-get install openfoam2106-default # change to openfoam2012-default for v2012
```

### WSL

```bash
    sudo apt-get update
    sudo apt-get install -y dos2unix ca-certificates cmake
    sudo find . -type f -exec dos2unix {} \; # required cloned on a windows machine
    curl -s https://dl.openfoam.com/add-debian-repo.sh | sudo bash
    wget -q -O - https://dl.openfoam.com/add-debian-repo.sh | sudo bash
    sudo apt-get install -y openfoam2106-default # change to openfoam2012-default for v2012
    sudo apt-get install -y libatlas-base-dev
```

After the installation on WSl or Liunx the OpenFOAM, the enviroment needs to be set with

```bash
    source <installation path>/OpenFOAM-v2106/etc/bashrc
```

In order to avoid sourcing the enviroment with every new terminal session, the above line can be added with a text editor at the bottom of the .bashrc

```bash
    alias of2106='source <installation path>/OpenFOAM-v2106/etc/bashrc'
    alias of2012='source <installation path>/OpenFOAM-v2012/etc/bashrc'
```

Another option is to define a alias in the bashrc. This way, multiple OpenFOAM version can be installed and switched easily by typing of2106 or of2012 in the terminal

##  How to install FMU4FOAM

Install OpenFoam
```bash
pip install oftest conan
./build-ECI4FOAM.sh # init submodules
./Allwmake
```

Install the FMU build tool

```bash
pip install fmu4foam
```

## run example FMU


requires that OpenFOAM is build on that machine

```bash
cd example/TempControlFlange
./Allrun
```

FMU4FOAM to quickly generate a library with testsuite and hosted documenation

zip or clone this library and edit it from there



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
