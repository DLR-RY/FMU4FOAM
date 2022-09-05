#!/bin/bash
cd "${0%/*}" || exit                                # Run from this directory

rm -r Example
fmu4foam build -f test-example.py -of OfCase/ --no-external-tool

python test-pyfmi.py > log