from pylab import *
import sys
import pdb
import matplotlib.pyplot as plt
import numpy as np
import pylab

from matplotlib import rc
from pylab import *

rc('text', usetex=True)
rc('font', family='serif')
font = {'family' : 'normal',\
                        'size'   : 18}
matplotlib.rc('font', **font)
w = 0.15
xlocations = np.array([1,1.5]) +w
def f(li):
    res = [li[0]]
    s = li[0]
    for i in range(1,len(li)):
        res.append(li[i]+s)
        s += li[i]
    return res

def plot_bar(vals,colors,labs,off=0):
    data = [f(x) for x in vals]
    xlocations = np.arange(len(vals))+off+0.15
    print "x clocations", xlocations
    oL = list()
    index=0
    for x,d in zip(xlocations, data):
        for c,value in zip(colors, sorted(d,reverse=True)):
            print c
            if index%len(data)==0 and off==0:
                b = bar(x,value,width=w, color=c, label=labels[index/len(data)])
            else:
                b = bar(x,value,width=w, color=c)
            index += 1
            oL.extend(b)
def decor(vals):
    plt.legend(loc=0,prop={'size':11})
    plt.ylabel("Time in seconds")
    xlocations = np.arange(len(vals))
    xlocations = np.array([0,1.0]) +w
    print xlocations
    pylab.gca().set_xticks(xlocations+w)
    pylab.gca().set_xticklabels([r'$\alpha = 0.5$',\
        r'$\alpha = 0.75$'])

colors = ['0.2','0.5','0.95']
#labels = ["NL Label", "Verification", "k-hop label"]
labels = ["NL label", "Verification", "khop Label"]
ct75 = [293.40, 127.965,224.455]
kt75 = [368.848, 653.14]
ct5 =  [119.847,35.5394, 13.71757]
kt5 =  [36.3393, 250.757]
plot_bar([ct5,ct75],colors,labels)
plot_bar([kt5,kt75], colors[1:], [],w)
decor([ct5,ct75])
plt.title('Effectiveness of labels on SCOP dataset')
plt.savefig(sys.argv[1],dpi=1000)

#w=0.5
#data = [f(t1), f(t3)]
#xlocations = np.array([1,1.5]) +w
#colors = ['0.2','0.5','0.8']
#oL = list()
#labels = ["Enumeration", "Hops", "Neighbors"]
#index=0
#for x,d in zip(xlocations, data):
    #for c,value in zip(colors, sorted(d,reverse=True)):
        #print c
        #if index%len(data)==0:
            #b = bar(x,value,width=w, color=c, label=labels[index/len(data)])
        #else:
            #b = bar(x,value,width=w, color=c)
        #index += 1
        #oL.extend(b)
#plt.legend(loc=2,prop={'size':11})
#plt.ylabel("Time in seconds")
#pylab.gca().set_xticks(xlocations+w/2)
#pylab.gca().set_xticklabels(["min_sup=10",\
#"min_sup=20","min_sup=25","min_sup=40"])
#pylab.gca().set_xticklabels(["Threshold = 0.1",\
#"Threshold = 0.3"])
#plt.savefig(sys.argv[1],dpi=1000)
#plt.setp(ax1.get_xticklabels(),visible=False)
#plt.bar(x,nbrs,color=colors[0])
#plt.bar(x,hops,color=colors[1])
#plt.bar(x,verify,color=colors[2])
