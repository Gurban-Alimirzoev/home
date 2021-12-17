import pandas as pd
from pathlib import Path

home = Path.cwd()

def ConvertExcel(path):
    path_xls = home / path
    print(path_xls)
    base = pd.read_excel(io = path_xls)
    print(base)
    pass

ConvertExcel('doc.xlsx')