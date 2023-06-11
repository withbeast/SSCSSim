import numpy as np
import matplotlib.pyplot as plt
plt.rc('font',family='Times New Roman',size=20)

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

def plot_spike(subfig,spk,title,xlabel=""):
    x=[]
    y=[]
    for i,v in enumerate(spk):
        if v>0:
            x.append(i)
            y.append(0)
    subfig.scatter(x,y, s=100, c="black", marker="|")
    subfig.set_ylim([-0.5,1])
    # subfig.set_ylabel(title,labelpad=27)
    subfig.set_title(title,y=0,pad=38)
    subfig.yaxis.set_ticks([])
    if(not xlabel == ""):
        subfig.set_xlabel(xlabel)

def plot_membrance(ax,mem,lim,thr_line=False):
    ax.plot(mem,c="black")
    ax.set_ylim(lim)
    ax.set_ylabel("Membrane Voltage (mV)")
    if thr_line:
        ax.axhline(y=thr_line, alpha=0.25, linestyle="dashed", c="black", linewidth=2)


def plot_cur_mem_spk(cur, mem, spk, thr_line=False, vline=False, title=False, ylim_max2=1.25):
    # Generate Plots
    fig, ax = plt.subplots(3, sharex=True,gridspec_kw={'height_ratios': [0.2, 0.4, 0.2]})
    # 画输入脉冲
    # plot_spike(ax[0],cur,"Input Spikes")
    # plot_currence(ax[0],cur,"Input Currence")
    plot_spike(ax[0],cur,"Input Spikes")
    # 画膜电位
    plot_membrance(ax[1],mem,[-0.1,0.8],0.5)
    # 画输出脉冲
    plot_spike(ax[2],spk,"Output Spikes","Time (ms)")

    plt.savefig("mem.pdf", bbox_inches='tight', pad_inches=0.02)

plot_cur_mem_spk(read_text("i.txt"),read_text("u.txt"),read_text("o.txt"))

