MAIN=server
SRC_C=$(wildcard src/*.c main.c)
BIN=bin
PARSER_BIN=bin/httpParser
PARSER_SRC=../../Release1/Sprint2/src

$(MAIN): $(SRC_C)
	gcc -I ../Doc/libparser -I ../Doc/librequest-0.5/api -L ../Doc/libparser -L ../Doc/librequest-0.5 -o bin/server main.c -lparser -lrequest

run: $(MAIN)
	./run.sh

clean:
	rm -f $(BIN)/$(MAIN)