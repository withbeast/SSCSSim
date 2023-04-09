import snntorch as snn
from snntorch import spikeplot as splt
from snntorch import spikegen
import numpy as np
import torch
import torch.nn as nn
import matplotlib.pyplot as plt


def read_text(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    datas = np.array(strs.split(","))
    return datas.astype(float)

def read_textb(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    arr=strs.split(",")
    datas = np.array(strs.split(","))
    return datas.astype(float)

def plot_spike(subfig,spk):
    x=[]
    y=[]
    for i,v in enumerate(spk):
        if v>0:
            x.append(i)
            y.append(0)
    subfig.scatter(x,y, s=100, c="black", marker="|")
    subfig.set_ylim([-0.5,0.5])
    subfig.set_ylabel("Output spikes")

def plot_membrance(ax,mem,thr_line=False):
    ax.plot(mem)
    ax.set_ylim([0, 1])
    ax.set_ylabel("Membrane Potential ($U_{mem}$)")
    if thr_line:
        ax.axhline(y=thr_line, alpha=0.25, linestyle="dashed", c="black", linewidth=2)


def plot_cur_mem_spk(cur, mem, spk, thr_line=False, vline=False, title=False, ylim_max2=1.25):
    # Generate Plots
    fig, ax = plt.subplots(3, sharex=True,gridspec_kw={'height_ratios': [0.4, 1, 0.4]})
    plot_spike(ax[0],cur)
    
    # Plot input current
    # ax[0].plot(cur, c="tab:orange")
    # ax[0].set_ylim([0, 0.4])
    # ax[0].set_xlim([0, 200])
    # ax[0].set_ylabel("Input Current ($I_{in}$)")
    # if title:
    #     ax[0].set_title(title)

    # Plot membrane potential
    plot_membrance(ax[1],mem)
    
    plot_spike(ax[2],spk)
    # ax[2].yticks([])

    plt.savefig("fig2.png")

plot_cur_mem_spk(read_text("i.txt"),read_text("u.txt"),read_textb("o.txt"))
# T = 0.2
# dt = 0.001
# t = np.arange(0, T, dt)
# plt.clf()
# # plt.plot(t,V,'-b')
# # plt.plot(t,V2,'-g')
# plt.plot(t, read_from_cpp("u.txt"), '-y')
# # plt.plot(t,read_from_cpp("euler.txt"),'-r')
# plt.title('Membrane potential')
# plt.legend(['V'])
# plt.show()
