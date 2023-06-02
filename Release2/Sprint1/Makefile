MAIN=server
SRC=$(wildcard src/*.c main.c)
PARSER_SRC=../../Release1/Sprint2/src/*.c

$(MAIN): $(SRC) $(PARSER_SRC)
	gcc -Wall -g -Wextra -L ../Doc/librequest-0.5 -o bin/server $^ -lrequest

run: $(MAIN) prepa
	./scripts/run.sh

valgrind: $(MAIN) prepa
	./scripts/run.sh "valgrind"

prepa:
	./scripts/prepa.sh

unprepa:
	./scripts/unprepa.sh

setup:
	./scripts/setupFCGI.sh

clean: unprepa
	rm -f $(BIN)/$(MAIN)