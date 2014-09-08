SRC=src
BIN=bin
CC=gcc
MAIN=cliente

compile: $(SRC)/*.c
	$(CC) $(SRC)/*.c -o $(MAIN);
	mkdir -p $(BIN);
	mv $(MAIN) $(BIN)/

run: compile
	$(BIN)/$(MAIN)
