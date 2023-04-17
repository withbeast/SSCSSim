import numpy as np
import matplotlib.pyplot as plt


def read_text(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    datas = np.array(strs.split(","))
    return datas.astype(float)

# def read_textb(name):
#     file = open(name, "r")
#     strs = file.read()
#     strs = strs.replace("\n", "")
#     arr=strs.split(",")
#     datas = np.array(strs.split(","))
#     return datas.astype(float)

def plot_currence(subfig,i,title):
    subfig.plot(i)
    subfig.set_ylim([0, 6])
    subfig.set_ylabel(title)

def plot_spike(subfig,spk,title):
    x=[]
    y=[]
    for i,v in enumerate(spk):
        if v>0:
            x.append(i)
            y.append(0)
    subfig.scatter(x,y, s=100, c="black", marker="|")
    subfig.set_ylim([-0.5,0.5])
    subfig.set_ylabel(title)

def plot_membrance(ax,mem,thr_line=False):
    ax.plot(mem)
    ax.set_ylim([0, 15])
    ax.set_ylabel("Membrane Potential")
    if thr_line:
        ax.axhline(y=thr_line, alpha=0.25, linestyle="dashed", c="black", linewidth=2)


def plot_cur_mem_spk(cur, mem, spk, thr_line=False, vline=False, title=False, ylim_max2=1.25):
    # Generate Plots
    fig, ax = plt.subplots(3, sharex=True,gridspec_kw={'height_ratios': [0.4, 1, 0.4]})
    # 画输入脉冲
    # plot_spike(ax[0],cur,"Input Spikes")
    plot_currence(ax[0],cur,"Input Currence")
    # 画膜电位
    plot_membrance(ax[1],mem)
    # 画输出脉冲
    plot_spike(ax[2],spk,"Output Spikes")

    plt.savefig("fig2.png")

plot_cur_mem_spk(read_text("i.txt"),read_text("u.txt"),read_text("o.txt"))

