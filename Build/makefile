.DEFAULT := all

OBJ := \
main.o \
mm_csv_parser.o \
rp5_csv_parser.o \
matrix_calculator.o \
smith_param_calculator.o

LIB := -lpthread
INC_PATH := ../Code/inc
PARSER_INC_PATH := ../Code/fast-cpp-csv-parser
SRC_PATH := ../Code/src

CC := g++
CC_FLAGS := -std=c++0x -g3

all: $(OBJ)
	@echo "Linking..."
	@$(CC) $(LINK_FLAGS) -o main $(OBJ) $(LIB)
	@echo "Done making."

%.o: $(SRC_PATH)/%.cpp
	@echo "Building $^.o..."
	@$(CC) -c $(CC_FLAGS) -I$(INC_PATH) -I$(PARSER_INC_PATH) $^

clean:
	@echo "..."
	@rm -rf *.o main ../Output/*
	@echo "Done cleaning."