#CC = g++ -g
#Include = -I/home/pranay/Downloads/gtest-1.6.0/include/gtest/
#Library = -L/home/pranay/Downloads/gtest-1.6.0/build/ -lpthread -lgtest

#test: gtest.cpp
	#$(CC) $(Include) $^ $(Library) -o $@
#clean:
	#rm test
EXEC = test
#SRC_FILES = gtest.cpp khop.cpp read_hop.cpp utils.cpp utils1.cpp\
			pattern.cpp config.cpp store.cpp logger.cpp preprocess.cpp\
			repr.cpp\
			stats.cpp Counter.cpp timer.cpp memusage.cpp
SRC_FILES = gtest.cpp Globals.cpp config.cpp store.cpp preprocess.cpp utils.cpp\
			utils1.cpp logger.cpp walks.cpp gapprox_embed.cpp pattern.cpp\
			compressed_embed.cpp khop.cpp read_hop.cpp repr.cpp mymatch.cpp\
			stats.cpp Counter.cpp timer.cpp memusage.cpp
CXX = g++ -std=gnu++0x -O3
CC = $(CXX)
DEBUG_LEVEL = -g
EXTRA_CCFLAGS = -Wall -Wno-sign-compare
CXXFLAGS = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CPPFLAGS = -I.. -I/home/pranay/Downloads/gtest-1.6.0/include/gtest/
LDFLAGS = -L/home/pranay/Downloads/gtest-1.6.0/build/# -lpthread -lgtest
LDLIBS  = -lgtest -lpthread
O_FILES = $(SRC_FILES:%.cpp=%.o)
all: $(EXEC)
	#$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
$(EXEC): $(O_FILES)
	 $(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS) 
	 #$(LINK.cc) -o $(EXEC) $(O_FILES) $(LDLIBS)
depend:
	makedepend -- $(CXXFLAGS) -- -Y $(SRC_FILES)
clean:
	$(RM) $(O_FILES)
