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

print 'Usage : python scop_minsup.py scop_minsup/scop_minsup.eps'

nbrs = [476.101,462.524,461.301,377.108]
hops = [489.592,513.252,625.521,378.995]
verify = [1188.25,1055.28,1148.42,629.804]
def f(li):
    res = [li[0]]
    s = li[0]
    for i in range(1,len(li)):
        res.append(li[i]+s)
        s += li[i]
    return res

s20=[476.101,489.592,1188.25]
s25=[462.524,513.252,1055.28]
s40=[461.301,625.521,1148.42]
s10=[377.108,378.995,629.804]

# because we are considering for only 300 max pats
#t5 = [i*.6 for i in [393.536,5954.34,32591.4]]
#t1 = [17.4463,36.069,0.814276]
#t3 = [88.422,670.379,365.958]
data=[f(s10),f(s20),f(s25),f(s40)]
#data = [f(t1), f(t3)]
w=0.2
xlocations = np.array([1,1.5,2,2.5]) +w
#xlocations = np.array([1,1.5]) +w
colors = ['0.2','0.5','0.8']
oL = list()
labels = [r'Verification', r'k-hop Label', r'NL Label']
index=0
for x,d in zip(xlocations, data):
    for c,value in zip(colors, sorted(d,reverse=True)):
        print c
        if index%len(data)==0:
            b = bar(x,value,width=w, color=c, label=labels[index/len(data)])
        else:
            b = bar(x,value,width=w, color=c)
        index += 1
        oL.extend(b)
plt.legend(loc=2,prop={'size':11})
plt.ylabel(r'Time in seconds')
plt.title(r'Time for random walks in SCOP dataset')
pylab.gca().set_xticks(xlocations+w/2)
pylab.gca().set_xticklabels([r'minsup=$10$',\
r'minsup=$20$',r'minsup=$25$',r'minsup=$40$'])
#pylab.gca().set_xticklabels(["Threshold = 0.1",\
#"Threshold = 0.3"])
plt.savefig(sys.argv[1],dpi=1000)
#plt.setp(ax1.get_xticklabels(),visible=False)
#plt.bar(x,nbrs,color=colors[0])
#plt.bar(x,hops,color=colors[1])
#plt.bar(x,verify,color=colors[2])
