EXEC = mine
#SRC_FILES = mine.cpp Globals.cpp db.cpp sim.cpp walk.cpp pattern.cpp mapping.cpp
#SRC_FILES = mine.cpp db.cpp walk.cpp pattern.cpp mapping.cpp sim.cpp Globals.cpp multiset.cpp \
			misc.cpp args.cpp print.cpp Globals.cpp parse.cpp read_hop.cpp
SRC_FILES = Globals.cpp mine.cpp config.cpp store.cpp preprocess.cpp utils.cpp\
			utils1.cpp logger.cpp
CXX = g++-4.4 -std=gnu++0x -O3 -fopenmp -DLOG_TRACE
CC = $(CXX)
DEBUG_LEVEL = -g
EXTRA_CCFLAGS = -Wall -Wno-sign-compare
CXXFLAGS = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CPPFLAGS = -I.
LDFLAGS = -L.
O_FILES = $(SRC_FILES:%.cpp=%.o)
all: $(EXEC)
$(EXEC): $(O_FILES)
depend:
	makedepend -- $(CXXFLAGS) -- -Y $(SRC_FILES)
clean:
	$(RM) $(O_FILES) $(EXEC)
