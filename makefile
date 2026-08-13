# Définition des variables
CC = g++
CFLAGS = -Wall
STD = -std=c++20
OPT = -O1

# Règle principale
fto: main.cpp permutation.hpp move.hpp fto.hpp utils.hpp
	$(CC) $(STD) $(OPT) main.cpp -o fto

test: fto_test.cpp permutation.hpp move.hpp fto.hpp utils.hpp
	$(CC) $(STD) $(OPT) fto_test.cpp -o test

# Règle de nettoyage
clean:
	rm -f fto test