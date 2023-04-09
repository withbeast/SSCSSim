import snntorch as snn
from snntorch import spikeplot as splt
from snntorch import spikegen
import numpy as np
import torch
import torch.nn as nn
import matplotlib.pyplot as plt
x=[1,5,7,8,10]
y=[0,0,0,0,0]
plt.scatter(x,y,300,'k','|')
plt.ylim([-0.5,0.5])
plt.savefig("test.png")