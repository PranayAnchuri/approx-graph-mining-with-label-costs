# compute the stats about the average size of patterns found
import networkx as nx
import sys
import pdb
import os
import os.path

class MyGraph(nx.Graph):
    def __init__(self,**kwds):
        self.sup = 0
        nx.Graph.__init__(self, kwds)
    def add_sup(self, sup):
        self.sup = sup

def avg(li):
# compute the avergae of the list
    if not li:
        return 0
    return float(sum(i for i in li))/len(li)

def get_stats(grs):
# compute various stats for the graphs returned
    vsize = [len(i) for i in grs]
    esize = [len(i.edges()) for i in grs]
    avgdeg = [avg([deg[1] for deg in gr.degree_iter()]) for gr in grs]
    return {'Avg Vsize': avg(vsize), 'Avg Esize':avg(esize), 'Avg\
            Degree':avg(avgdeg), 'Max Size': max(vsize), 'Max Edges':max(esize)}


def is_graph_line(line):
    return line.startswith("v") or line.startswith("e") or line.startswith("Support")

def add_graph_line(gr, line):
    sp = line.split(" ")
    if sp[0] == 'v':
        gr.add_node(int(sp[1]), label=sp[2])
    elif sp[0] == 'e':
        gr.add_edge(int(sp[1]), int(sp[2]), label=sp[3])
    elif sp[0].startswith("Support"):
        gr.add_sup(int(sp[1]))
    else:
        raise Exception

def parse_input(inp):
    reading_a_graph = False
    f = open(inp)
    for line in f.readlines():
        line = line.strip()
        if not reading_a_graph and is_graph_line(line):
            gr = MyGraph()
            add_graph_line(gr, line)
            reading_a_graph = True

# this value is computed already but who cares about the efficiency
        if is_graph_line(line):
            add_graph_line(gr, line)
        else:
            if reading_a_graph:
                reading_a_graph = False
                yield gr
            else:
                continue

def write_stats(stats, out):
    w = open(out, 'w')
    for inp, stat in stats.items():
        st = "\n"+inp+"\n"
        st += '\n'.join([str(k)+"\t"+str(v) for k,v in stat.items()])
        w.write(st)
    w.close()
    
def process(inpdir, outfile):
    all_stats = {}
    for inp in os.listdir(inpdir):
        grs = parse_input(os.path.join(inpdir, inp))
        if inp.startswith("ppi"):
            grs = [gr for gr in grs if gr.sup==3]
        all_stats[inp] = get_stats([gr for gr in grs])
    write_stats(all_stats, outfile)

if __name__ == '__main__':
    #grs =[i for i in parse_input(sys.argv[1])]
    process(sys.argv[1], sys.argv[2])
    pdb.set_trace()
