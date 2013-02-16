import sys
import pdb
from plot import *

from collections import deque

def process(inp, outfile):
    lines = deque([i.strip() for i in open(inp).readlines()])
    def proc_line(li):
        sp = li.split(":")
        return float(sp[0]), [float(x) for x in sp[1].split(",")]
    def read_two(qu):
        l1 = qu.popleft()
        l2 = qu.popleft()
        lab1, values1 = proc_line(l1)
        lab2, values2 = proc_line(l2)
        return [values1, values2], [r'NL pruning', r' no pruning']
    values = []
    xvalues = [i*10 for i in range(1, 51)]
    while lines:
        ret = read_two(lines)
        values.append((xvalues, ret[0], ret[1]) )
    titles = [0.01, 0.7, 1.0, 1.5]
    mymultplot(values, [r'Number of random walks', r'Time in seconds',\
                        titles], outfile)

    pdb.set_trace()

if __name__ == '__main__':
    print 'python scop_alpha_effect.py scop_alpha_effect/5F20P.stats scop_alpha_effect/5F20P.png'
    process(sys.argv[1], sys.argv[2])

