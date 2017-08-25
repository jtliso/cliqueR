CC = g++
CFLAGS = -O3
OBJS = Bitarray.o Brancher.o Candidate_Manager.o Clique_Test.o Degree_Preprocessor.o Graph.o MC_Heuristic.o Preprocessor.o bit_counters.o color.o paracl.o
EXECUTABLES = paracl_cp

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

all: $(EXECUTABLES)

paracl_cp: paracl_cp.cpp $(OBJS)
	$(CC) $(CFLAGS) -o paracl_cp paracl_cp.cpp $(OBJS) $(PARACLOBJS)

clean:
	rm -f $(EXECUTABLES) *.o
