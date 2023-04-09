import numpy as np

a=np.zeros(10)
a[8]=1
a[3]=1
b=np.where(a==1)[0]
print(b)