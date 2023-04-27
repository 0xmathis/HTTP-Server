MAIN=server
SRC=$(wildcard src/*.c main.c)
PARSER_SRC=../../Release1/Sprint2/src/*.c


$(MAIN): $(SRC) $(PARSER_SRC)
	gcc -Wall -Wextra -L ../Doc/librequest-0.5 -o bin/server $^ -lrequest

run: $(MAIN)
	./run.sh

clean:
	rm -f $(BIN)/$(MAIN)