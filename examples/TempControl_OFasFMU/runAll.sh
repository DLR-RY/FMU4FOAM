#!/bin/bash
cd "${0%/*}" || exit                                # Run from this directory

rm -r TempControl
fmu4foam build -f TempControl.py -of OfCase/ --no-external-tool

python runFMUs.py > log