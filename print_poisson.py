import snntorch as snn
from snntorch import spikeplot as splt
from snntorch import spikegen
import numpy as np
import torch
import torch.nn as nn
import matplotlib.pyplot as plt
num_steps=200
def plot_snn_spikes(spk_in, spk1_rec, spk2_rec, title):
  # Generate Plots
  fig, ax = plt.subplots(3, figsize=(8,7), sharex=True, 
                        gridspec_kw = {'height_ratios': [1, 1, 0.4]})

  # Plot input spikes
  splt.raster(spk_in[:,0], ax[0], s=0.03, c="black")
  ax[0].set_ylabel("Input Spikes")
  ax[0].set_title(title)

  # Plot hidden layer spikes
  splt.raster(spk1_rec.reshape(num_steps, -1), ax[1], s = 0.05, c="black")
  ax[1].set_ylabel("Hidden Layer")

  # Plot output spikes
  splt.raster(spk2_rec.reshape(num_steps, -1), ax[2], c="black", marker="|")
  ax[2].set_ylabel("Output Spikes")
  ax[2].set_ylim([0, 10])

  plt.show()

def read_text(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    datas = np.array(strs.split(","))
    return datas.astype(int)

plot_snn_spikes()
