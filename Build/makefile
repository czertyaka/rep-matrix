.DEFAULT := all

OBJ := main.o matrix.o observation.o
INC_PATH := ../Code/inc
SRC_PATH := ../Code/src

CC := g++
CC_FLAGS := -g3

all: $(OBJ)
	@echo "Linking..."
	@$(CC) $(LINK_FLAGS) -o main $(OBJ)
	@echo "Done making."

%.o: $(SRC_PATH)/%.cpp
	@echo "Building $^.o..."
	@$(CC) -c $(CC_FLAGS) -I$(INC_PATH) $^

clean:
	@echo "..."
	@rm -rf *.o main
	@echo "Done cleaning."