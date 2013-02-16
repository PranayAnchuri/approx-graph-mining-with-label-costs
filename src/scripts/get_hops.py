import networkx as nx
import sys
import pdb
from collections import defaultdict
from collections import Counter
from collections import deque
import numpy as np
import os
import os.path

class Multiset:

    def __init__(self,x,lab):
        self.vid = x
        self.hops = {}
        self.hops[0] = {lab:1}

    def add_hop(self, ht, ms):
        self.hops[ht] = ms

    def tostr(self):
        # get the str representation of the multiset
        st = "%s\n"%(str(self.vid))
        for k,v in self.hops.items():
            st += "%s %s\n"%(str(k),' '.join([str(lab)+":"+str(cnt) for lab,cnt\
                in v.items()]))
        return st

    def myprint(self):
        print self.tostr()

def read_gr(f):
    gr = nx.Graph()
    for li in open(f):
        li = li.strip()
        sp = li.split(" ")
        if sp[0].startswith("v"):
            gr.add_node(int(sp[1]), label=sp[2])
        if sp[0].startswith("e") or sp[0].startswith("u"):
            gr.add_edge(int(sp[1]), int(sp[2]))
    return gr

def get_max_diam(comp):
    # for each vertex in the component, get the maximum distance a vertex is at
    pdb.set_trace()
    sp = nx.shortest_path(comp)
    return dict(zip(comp.nodes(), [max([len(sp[i][v])-1 for v in sp[i].keys()]) for\
        i in comp.nodes()]))

    print len(comp.nodes())
def diagonal(mat):
    mat.setdiag(np.zeros(mat.shape[0]))

def reachable(comp, visited, top, depth):
    if depth ==0:
        return set([top])
    else:
        # get the neighbors of the last element
        ret = set([])
        for nbr in comp.neighbors(top):
            if nbr not in visited:
                partial = reachable(comp, visited.union([nbr]), nbr, depth-1 )
                ret.update(partial)
        return ret

def naive_hops(comp, maxlevel=5):
    # get the khops for all the vertices in the component
    #hops = dict(zip(comp.nodes(), [Multiset(i,attr["label"]) for i,attr in comp.nodes(data=True)]))
    hopvals = [Multiset(i,attr["label"]) for i,attr in comp.nodes(data=True)]
    hops = dict(zip([x.vid for x in hopvals], hopvals))
    #maxdist = get_max_diam(comp)
    #return naive_hops()
    for index,node in enumerate(comp.nodes()):
        for level in range(1,maxlevel):
            if level > maxlevel:
                continue
            #labs = [comp.node[i]['label'] for i in reachable(comp, set([node]),\
                #node, level) ]
            vreach = reachable(comp, set([node]), node, level)
            #labs = [comp.node[i]['label'] for i in reachable(comp, set([node]),\
                #node, level) if len(i) > 0 ]
            if len(vreach) > 0:
                labs = [comp.node[i]['label'] for i in vreach]
                labels = Counter(labs)
                hp = dict(zip(labels.keys(), labels.values()))
                hops[node].add_hop(level, hp)
            else:
                break
        print index
    return hops


def get_hops(comp):
    maxdist = get_max_diam(comp)
    assert(nx.diameter(comp) == max(maxdist.values()))
    dm = nx.diameter(comp)
    # create hop objects for each node
    hops = dict(zip(comp.nodes(), [Multiset(i,attr["label"]) for i,attr in comp.nodes(data=True)]))
    spm = nx.to_scipy_sparse_matrix(comp)
    pows = spm
    indices = dict(zip(comp.nodes(), range(len(comp.nodes()))))
    inv_indices = dict([(v,k) for k,v in indices.items()])
    once = False
    for itr in range(1,dm+1):
        # get the vertices
        for v,diam in maxdist.items():
            if itr <= diam:
                # get for this lev
                rw = np.array(pows.getrow(indices[v]).todense())[0,:]
                # get the multiset
                #pdb.set_trace()
                try:
                    labs = [comp.node[inv_indices[index]]['label'] for index,paths\
                            in enumerate(rw) if paths>0]
                except KeyError:
                    pdb.set_trace()
                #pdb.set_trace()
                labels = Counter(labs)
                hp = dict(zip(labels.keys(), labels.values()))
                hops[v].add_hop(itr, hp)
        pows = pows*spm
        if not once:
            # make the diagonal elements 0
            #diagonal(pows)
            once = True
    return hops

def compute_hops(f, o):
    gr = read_gr(f)
    w = open(o,'w') # file to which the components are written
    # get the connected components
    cgrs = nx.connected_component_subgraphs(gr)
    for i in cgrs:
        #hps = get_hops(i)
        #hps = get_hops_naive(i)
        print "Component"
        hps = naive_hops(i)
        #pdb.set_trace()
        [w.write(x.tostr()) for x in hps.values()]
    w.close()

def convert_dir(inp):
    # get the hops for all the files in the directory
    for f in os.listdir(inp):
        filename, ext = os.path.splitext(os.path.join(inp,f))
        if ext=='.txt':
            compute_hops(os.path.join(inp,f), filename+".khops")

if __name__=='__main__':
    gr = read_gr(sys.argv[1])
    compute_hops(sys.argv[1], sys.argv[2])
    #convert_dir(sys.argv[1])
    '''
    gr = nx.Graph()
    gr.add_node(1,label="A")
    gr.add_node(2,label="B")
    gr.add_node(3,label="B")
    gr.add_node(4,label="C")
    gr.add_edge(1,2)
    gr.add_edge(1,3)
    gr.add_edge(2,3)
    gr.add_edge(1,4)
    hops = naive_hops(gr,5)
    '''
