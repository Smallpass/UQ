import pandas as pd

Time = [0,1,2,3,4,5]
ds = pd.DataFrame(Time)  
ds.to_excel('CompProjectSim.xlsx')