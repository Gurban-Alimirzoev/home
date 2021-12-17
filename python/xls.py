import pandas as pd
def ConvertExcel(path):
    base = pd.read_excel(path, sheet_name=[x for x in range(3)],  index_col = None)
    new_base = pd.concat(base.values(), ignore_index=True)
    new_base.to_excel("new_Upwork.xlsx", index=False)
    pass

ConvertExcel('Upwork.xlsx')