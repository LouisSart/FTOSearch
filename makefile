# Définition des variables
CC = g++
CFLAGS = -Wall
STD = -std=c++20
OPT = -O1

# linking
fto: main.o permutation.o move.o utils.o
	$(CC) $(STD) $(OPT) main.o permutation.o move.o utils.o -o fto

test: test.o permutation.o move.o utils.o
	$(CC) $(STD) $(OPT) test.o permutation.o move.o utils.o -o test


# Compilation
main.o: main.cpp
	$(CC) $(STD) $(OPT) -c main.cpp -o main.o

test.o: fto_test.cpp
	$(CC) $(STD) $(OPT) -c fto_test.cpp -o test.o

permutation.o : permutation.cpp
	$(CC) $(STD) $(OPT) -c permutation.cpp -o permutation.o

move.o : move.cpp
	$(CC) $(STD) $(OPT) -c move.cpp -o move.o

utils.o : utils.cpp
	$(CC) $(STD) $(OPT) -c utils.cpp -o utils.o

# Règle de nettoyage
clean:
	rm -f *.o fto test