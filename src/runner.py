import pdb
import os
import sys
import os.path
import commands

def ensure(directory):
    if not os.path.exists(directory):
            os.makedirs(directory)

DS = "../results/SCOP_Complete/"
DB = "../data/SCOP/5F20P/5F20P.txt"
SIM = "../data/SCOP/5F20P/5F20P.sim"
HOPS = "../data/SCOP/5F20P/5F20P.khops"
MULTI  = "1"
OFFSET = "../data/SCOP/5F20P/5F20P.off"

WALKS = [500]
ALPHA  = [1]
MINSUP  = [15]
SEED  = [1729]
ALGO = [0] # 1 is compressed label and 0 is complete enumeration


for walk in WALKS:
    for alpha in ALPHA:
        for minsup in MINSUP:
            for seed in SEED:
                for algo in ALGO:
                    ensure(DS)
                    filesuffix = "W%sE%sM%sS%sA%s.txt"%(str(walk), str(alpha),\
                                    str(minsup), str(seed), str(algo))
                    confile = os.path.join(DS, "Config_"+filesuffix)
                    statfile = os.path.join(DS, "Stat_"+filesuffix)
                    logfile = os.path.join(DS, "Log_"+filesuffix)
                    f1 = open(confile, 'w')
                    attrs = ["DB", "SIM", "HOPS", "WALKS", "ALPHA", "SEED", "MINSUP", "ALGO", "MULTI", "OFFSET"]
                    values = [DB, SIM, HOPS, walk, alpha, seed, minsup, algo,MULTI, OFFSET]
                    #attrs = ["DB", "SIM", "HOPS", "WALKS", "ALPHA", "SEED", "MINSUP", "ALGO"]
                    #values = [DB, SIM, HOPS, walk, alpha, seed, minsup, algo]
                    f1.write('\n'.join([i+" "+str(j) for i,j in zip(attrs,\
                        values)]))
                    f1.close()
                    cmd = './mine %s %s %s'%(confile, logfile, statfile)
                    print cmd
                    commands.getstatusoutput(cmd)

if __name__ == '__main__':
    pass

