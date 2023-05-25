MAIN=server
#SRC=$(wildcard src/*.c main.c)
SRC=$(wildcard src/Mathis.c main.c)
PARSER_SRC=../../Release1/Sprint2/src/*.c

$(MAIN): $(SRC) $(PARSER_SRC)
	gcc -Wall -Wextra -L ../Doc/librequest-0.5 -o bin/server $^ -lrequest

run: $(MAIN) prepa
	./scripts/run.sh

valgrind: $(MAIN) prepa
	./scripts/run.sh "valgrind"

clean:
	rm -f $(BIN)/$(MAIN)

prepa:
	./scripts/prepa.sh

unprepa:
	./scripts/unprepa.sh
