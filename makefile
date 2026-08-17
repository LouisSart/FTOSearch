# Définition des variables
# CC = g++
CFLAGS = -Wall -I/src
STD = -std=c++20
SRC_DIR = src
OPT = -O1 -I$(SRC_DIR)/
OBJ_DIR = obj
EXE = $(OBJ_DIR)/fto
TEST = $(OBJ_DIR)/test
MAIN_SRC = $(SRC_DIR)/main.cpp
TEST_SRC = $(SRC_DIR)/test.cpp

SRC := $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/test.cpp, $(wildcard $(SRC_DIR)/*.cpp)) # Tous les fichiers sources sauf main et test
HDR := $(filter-out $(SRC_DIR)/main.hpp $(SRC_DIR)/test.hpp, $(wildcard $(SRC_DIR)/*.hpp)) # Tous les headers sauf main et test
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) # Tous les fichiers objets

all : fto test

# linking
fto: $(OBJ) $(OBJ_DIR)/main.o
	$(CXX) $(STD) $(OPT) $(OBJ_DIR)/main.o $(OBJ) -o $(EXE)

test: $(OBJ) $(OBJ_DIR)/test.o
	$(CXX) $(STD) $(OPT) $(OBJ_DIR)/test.o $(OBJ) -o $(TEST)


# Compilation
$(OBJ_DIR)/main.o: $(MAIN_SRC) $(HDR)
	$(CXX) $(STD) $(OPT) -c $(MAIN_SRC) -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/test.o: $(TEST_SRC) $(HDR)
	$(CC) $(STD) $(OPT) -c $(TEST_SRC) -o $(OBJ_DIR)/test.o


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(STD) $(OPT) -c $< -o $@

.PHONY: all clean

# Règle de nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/fto $(OBJ_DIR)/test