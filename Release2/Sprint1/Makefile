MAIN=server
#SRC=$(wildcard src/*.c main.c)
SRC=$(wildcard src/Mathis.c main.c)
PARSER_SRC=../../Release1/Sprint2/src/*.c

$(MAIN): $(SRC) $(PARSER_SRC)
	gcc -Wall -Wextra -L ../Doc/librequest-0.5 -o bin/server $^ -lrequest

run: $(MAIN)
	./run.sh

clean:
	rm -f $(BIN)/$(MAIN)

prepa:
	sudo mv /etc/hosts /etc/hosts.temp
	sudo cp hosts /etc/hosts

unprepa:
	sudo mv /etc/hosts.temp /etc/hosts
