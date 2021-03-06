EXEC = mine
OUTDIR = obj
#SRC_FILES = mine.cpp Globals.cpp db.cpp sim.cpp walk.cpp pattern.cpp mapping.cpp
#SRC_FILES = mine.cpp db.cpp walk.cpp pattern.cpp mapping.cpp sim.cpp Globals.cpp multiset.cpp \
			misc.cpp args.cpp print.cpp Globals.cpp parse.cpp read_hop.cpp
SRC_FILES = Globals.cpp mine.cpp config.cpp store.cpp preprocess.cpp utils.cpp\
			utils1.cpp logger.cpp walks.cpp gapprox_embed.cpp pattern.cpp\
			compressed_embed.cpp khop.cpp read_hop.cpp repr.cpp mymatch.cpp\
			stats.cpp Counter.cpp timer.cpp memusage.cpp \
			defs.cpp graph.cpp partition.cpp orbit.cpp uintseqhash.cpp heap.cpp\
 			nauty_timer.cpp nauty_utils.cpp #bliss_C.cpp
CXX = g++-4.4 -std=gnu++0x -O3 -fopenmp -DLOG_TRACE
CC = $(CXX)
DEBUG_LEVEL = -g
EXTRA_CCFLAGS = -Wall -Wno-sign-compare
CXXFLAGS = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CPPFLAGS = -I.
LDFLAGS = -L.
O_FILES = $(SRC_FILES:%.cpp=%.o)
#NAUTY_O_FILES = $(SRC_NAUTY:%.cpp=%.o)
OBJS_O= $(foreach obj, $(O_FILES), $(OUTDIR)/$(obj) )
all: $(EXEC)
#$(EXEC): $(O_FILES)
$(EXEC): $(O_FILES)
depend:
	makedepend -- $(CXXFLAGS) -- -p $(OUTDIR)/ -Y $(SRC_FILES) 
clean:
	#$(RM) $(O_FILES) $(EXEC)
	rm -f *.o
