MAIN=server
SRC=$(wildcard src/*.c main.c)
PARSER_SRC=../Sprint2/src/*.c

$(MAIN): $(SRC) $(PARSER_SRC)
	gcc -Wall -g -Wextra -L doc/librequest-0.5 -o bin/server $^ -lrequest

run: $(MAIN) prepa
	./scripts/run.sh

valgrind: $(MAIN) prepa
	./scripts/run.sh "valgrind"

prepa:
	./scripts/prepa.sh

unprepa:
	./scripts/unprepa.sh

setupFCGI:
	./scripts/setupFCGI.sh

clean: unprepa
	rm -f $(BIN)/$(MAIN)