import os
import pytest
import pandas as pd
import oftest
from oftest import run_reset_case, run_case
import json

# NOTE (2 2 2) is converted by openfoam to ( 2 2 2 ) note the spaces

def test_dictMod(run_reset_case):
    log = oftest.path_log()

    json_path = oftest.base_dir() + "/" + "dictMod.json"
    with open(json_path) as f:
        data = json.load(f)

    for key, value in data.items():
        file_name = oftest.base_dir() + "/" + key.split(";")[0]
        with open(file_name) as f:
            file_content = f.read()
            assert value in file_content

    assert oftest.case_status(log) == 'completed'


