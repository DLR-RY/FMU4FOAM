import os
import pytest
import pandas as pd
import oftest
from oftest import run_reset_case


def test_completed(run_reset_case):
    log = oftest.path_log()
    assert oftest.case_status(log) == 'completed'
    fpath = "{}/controlledTemperature.csv".format(oftest.base_dir())
    print(fpath)
    T = pd.read_csv(fpath,skiprows=1)
    T["diff"] = abs(T[' "model.root.system1.Tin"']-T[' "model.root.system1.ramp.y"'])
    assert T["diff"].max() < 2