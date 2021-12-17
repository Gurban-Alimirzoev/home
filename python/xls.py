import pandas as pd
from pathlib import Path

home = Path.cwd()

def ConvertExcel(path):
    path_xls = home / path
    print(path_xls)
    base = pd.read_excel(path)
    print(base)
    pass
print("Gurban LOH\n"*10000)
ConvertExcel('Upwork.xlsx')