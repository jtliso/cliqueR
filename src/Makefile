CC = g++
CFLAGS = -O3
OBJS = Bitarray2.o Brancher2.o Candidate_Manager2.o Clique_Test2.o Degree_Preprocessor2.o Graph2.o MC_Heuristic2.o Preprocessor2.o bit_counters2.o color2.o paracl.o
EXECUTABLES = paracl_cp

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

all: $(EXECUTABLES)

paracl_cp: paracl_cp.cpp $(OBJS)
	$(CC) $(CFLAGS) -o paracl_cp paracl_cp.cpp $(OBJS) $(PARACLOBJS)

clean:
	rm -f $(EXECUTABLES) *.o
