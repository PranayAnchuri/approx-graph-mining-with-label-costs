import pdb
import os
import os.path
import sys
import matplotlib.pyplot as plt
from matplotlib import rc
from pylab import *

rc('text', usetex=True)
rc('font', family='serif')

linestyles = ['_', '-', '--', ':']
markers = []
for m in Line2D.markers:
    try:
        if len(m) == 1 and m != ' ':
            markers.append(m)
    except TypeError:
        pass

styles = markers + [
        r'$\lambda$',
        r'$\bowtie$',
        r'$\circlearrowleft$',
        r'$\clubsuit$',
        r'$\checkmark$']

font = {'size'   : 18}
matplotlib.rc('font', **font)
def verify(values):
    '''Returns true if the input to the plot function can be parsed and the 
    values can be plotted'''
    for stat in values:
        if len(stat) != 3:
            return False
    return True

'''
Requires :
    1) myvalues : list of 3 tuples, (xvalues, yvalues, lab)
    2) outfile : file to which the output is saved
    3) labels : xlabel, ylabel and the title
'''
def prune_vals(xvalues, yvalues, step=4):
    if len(xvalues) != len(yvalues):
        pdb.set_trace()
        raise "values cannot be pruned"
    cmn_len = len(xvalues)
    return [xvalues[i] for i in range(0, cmn_len, step)], \
            [yvalues[i] for i in range(0, cmn_len, step)]
    

def myplot(values, labels,  outfile):
    if not verify(values):
        print 'input cannot be parsed'
        pdb.set_trace()
    indices = [9, 3, 8]
    print styles
    for index, stat in enumerate(values):
        #plt.plot(stat[0], stat[1], 'k'+styles[indices[index]],  label=stat[2])
        x, y = prune_vals(stat[0], stat[1])
        plt.plot(x, y, 'k'+styles[indices[index]],  label=stat[2])
    mysave(labels, outfile)

'''
Requires:
    1) values : list of 3 tuples, (xvalues, list of yvalues, list of labels)
'''
def mymultplot(values, labels, outfile):
    if not verify(values):
        print 'input cannot be parsed'
        pdb.set_trace()
    indices = [21, 2, 8, 4, 8]
    numrows = 2
    ncols = 2
    count = 1
    pdb.set_trace()
    for index, stat in enumerate(values):
        subplot(numrows, ncols, count)
        try:
            l = len(stat[1])
            print l
            for i in range(l):
                x, y = prune_vals(stat[0], stat[1][i])
                plt.plot(x, y, 'k'+styles[indices[i]], label = stat[2][i])
                #plt.plot(stat[0], stat[1][i], 'k'+styles[indices[i]], label = stat[2][i])
                #plt.plot(stat[0], stat[1][i], label = stat[2][i])
            plt.xlabel(labels[0])
            plt.ylabel(labels[1])
            print index, labels[2][index],r'$ \alpha = %s $'%str(labels[2][index])
            plt.title(r'$ \alpha = %s $'%str(labels[2][index]))
            plt.legend(loc=2, numpoints=1)
        except KeyError:
            pdb.set_trace()
        count += 1
    plt.tight_layout()
    mysave(labels, outfile)

def mysave(labels, outfile):
    plt.legend(loc=2, numpoints=1)
    try:
        pass
        plt.xlabel(labels[0])
        plt.ylabel(labels[1])
        plt.title(labels[2])
    except IndexError:
        print 'labels not set'
    plt.savefig(outfile, dpi=500)
