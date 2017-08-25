CC = g++
CFLAGS = -O3
MPIFLAGS = -lmpi -lmpi++ -DPARALLEL
OBJS = Bitarray.o Brancher.o Candidate_Manager.o Clique_Test.o Degree_Preprocessor.o Graph.o MC_Heuristic.o Preprocessor.o bit_counters.o color.o match.o
PAROBJS = parMPI.o PBrancher.o
PARACLOBJS = paracl.o
GRAPHOBJS = Bitarray.o Graph.o bit_counters.o
EXECUTABLES = clique mcq paracl_ha paracl_ho paracl_so paracl_sa density

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

all: $(EXECUTABLES)


# DIRECT CLIQUE BINARIES

# Standard direct clique for biology-related graphs
clique: clique.cpp $(OBJS)
	$(CC) $(CFLAGS) -o clique clique.cpp $(OBJS)

# Direct clique using MCQ strategy of Tomita and Kameda
mcq: mcq.cpp $(OBJS)
	$(CC) $(CFLAGS) -o mcq mcq.cpp $(OBJS)

# PARACLIQUE BINARIES

# Hard original paraclique
paracl_ho: paracl_ho.cpp $(OBJS) $(PARACLOBJS)
	$(CC) $(CFLAGS) -o paracl_ho paracl_ho.cpp $(OBJS) $(PARACLOBJS)

# Hard accelerated paraclique
paracl_ha: paracl_ha.cpp $(OBJS) $(PARACLOBJS)
	$(CC) $(CFLAGS) -o paracl_ha paracl_ha.cpp $(OBJS) $(PARACLOBJS)

# Soft original paraclique
paracl_so: paracl_so.cpp $(OBJS) $(PARACLOBJS)
	$(CC) $(CFLAGS) -o paracl_so paracl_so.cpp $(OBJS) $(PARACLOBJS)

# Soft accelerated paraclique
paracl_sa: paracl_sa.cpp $(OBJS) $(PARACLOBJS)
	$(CC) $(CFLAGS) -o paracl_sa paracl_sa.cpp $(OBJS) $(PARACLOBJS)

# Density calculator
density: density.cpp $(GRAPHOBJS)
	$(CC) $(CFLAGS) -o density density.cpp $(GRAPHOBJS)

clean:
	rm -f $(EXECUTABLES) *.o
