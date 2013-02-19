# compare the run times for label pruning, compleete enumeration and gapprox
import sys
import pdb
from plot import *

def process(inp, out):
    xvalues = [10*i for i in range(1,51)]
    def read_line(line):
        line = line.strip()
        label, times = line.split(":")
        if len(xvalues) != len(times.split()):
            pdb.set_trace()
        return (xvalues, [float(x)/60.0 for x in times.split()], label)
    f = open(inp)
    values = []
    for line in f.readlines():
        values.append(read_line(line))
    labels = [r'Number of random walks',r'Time in minutes',r'Runtime comparison on SCOP dataset']
    myplot(values,labels, out)

if __name__ == '__main__':
    process(sys.argv[1], sys.argv[2])
