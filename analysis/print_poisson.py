import numpy as np
import matplotlib.pyplot as plt


def read_text(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    datas = np.array(strs.split(","))
    return datas.astype(float)

def plot_spike(spk,title):
    fig=plt.figure(figsize=(6,1),facecolor='white')
    x=[]
    y=[]
    for i,v in enumerate(spk):
        if v>0:
            x.append(i)
            y.append(0)
    plt.scatter(x,y, s=100, c="black", marker="|")
    plt.ylim([-0.5,0.5])
    plt.ylabel(title)
    plt.savefig("poisson.png")

plot_spike(read_text("po.txt"),"Poisson Spike")