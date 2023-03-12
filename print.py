import numpy as np
import matplotlib.pyplot as plt


def read_text(name):
    file = open(name, "r")
    strs = file.read()
    strs = strs.replace("\n", "")
    datas = np.array(strs.split(","))
    return datas.astype(float)


def plot_cur_mem_spk(cur, mem, spk, thr_line=False, vline=False, title=False, ylim_max2=1.25):
    # Generate Plots
    fig, ax = plt.subplots(3, sharex=True,gridspec_kw={'height_ratios': [1, 1, 0.4]})

    # Plot input current
    ax[0].plot(cur, c="tab:orange")
    ax[0].set_ylim([0, 0.4])
    ax[0].set_xlim([0, 200])
    ax[0].set_ylabel("Input Current ($I_{in}$)")
    if title:
        ax[0].set_title(title)

    # Plot membrane potential
    ax[1].plot(mem)
    ax[1].set_ylim([0, ylim_max2])
    ax[1].set_ylabel("Membrane Potential ($U_{mem}$)")
    if thr_line:
        ax[1].axhline(y=thr_line, alpha=0.25, linestyle="dashed", c="black", linewidth=2)
    plt.xlabel("Time step")
    # print(spk)
    ax[2].plot(spk,'o')
    # Plot output spike using spikeplot
    # plt.raster(spk, ax[2], s=400, c="black", marker="|")
    if vline:
        ax[2].axvline(x=vline, ymin=0, ymax=6.75, alpha=0.15, linestyle="dashed", c="black", linewidth=2, zorder=0,
                      clip_on=False)
    ax[2].set_ylim([0,2])
    ax[2].set_ylabel("Output spikes")
    # ax[2].yticks([])

    plt.savefig("fig.png")

plot_cur_mem_spk(read_text("i.txt"),read_text("u.txt"),read_text("o.txt"))
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
