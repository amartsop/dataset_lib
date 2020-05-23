import os
import time
import matplotlib.pyplot as plt 
from matplotlib import rc


def read_file(filename):
    lineList = [line.rstrip('\n') for line in open(filename)]
    return lineList

def fig_init(fig_num):
    plt.figure(num=int(fig_num));
    return 0;

def plot2D(vec1_path, vec2_path, fmt, lin, mark, lab):
    x = read_file(vec1_path); x = [float(i) for i in x]
    y = read_file(vec2_path); y = [float(i) for i in y]
    plt.plot(x, y, fmt, linewidth=int(lin), markersize=int(mark), label=lab); 
    if (lab):
        plt.legend()
    os.remove(vec1_path); os.remove(vec2_path)
    return 0;

def set_xlabel(in_text, latex_state, in_fontsize, in_font):
    plt.rc('text', usetex=(latex_state==("True")))
    plt.rc('font', family=in_font)
    plt.xlabel(in_text,fontsize=in_fontsize)
    return 0;

def set_ylabel(in_text, latex_state, in_fontsize, in_font):
    plt.rc('text', usetex=(latex_state==("True")))
    plt.rc('font', family=in_font)
    plt.ylabel(in_text,fontsize=in_fontsize)
    return 0;

def set_title(in_text, latex_state, in_fontsize, in_font):
    plt.rc('text', usetex=(latex_state==("True")))
    plt.rc('font', family=in_font)
    plt.title(in_text,fontsize=in_fontsize)
    return 0;

def subplot(rows, cols, fig):
    plt.subplot(int(rows), int(cols), int(fig))
    return 0;

def show(arg):
    plt.show()
    return 0;

def grid(arg):
    plt.grid()
    return 0;

def savefig(filepath):
    plt.savefig(filepath)
    return 0