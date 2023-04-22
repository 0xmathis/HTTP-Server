MAIN := bin/httpParser
SRC_C := $(wildcard src/*.c main.c)

$(MAIN): $(SRC_C)
	gcc -Wall $^ -o $@

shared: $(SRC_C)
	gcc -Wall -c -fPIC src/api.c -o bin/api.o
	gcc -Wall -c -fPIC src/Hugo.c -o bin/Hugo.o
	gcc -Wall -c -fPIC src/Josias.c -o bin/Josias.o
	gcc -Wall -c -fPIC src/Mathis.c -o bin/Mathis.o
	gcc -Wall -c -fPIC src/Nathan.c -o bin/Nathan.o
	gcc -Wall -c -fPIC src/Node.c -o bin/Node.o
	gcc -Wall -c -fPIC src/utils.c -o bin/utils.o
	gcc -shared -o bin/httpParser.so bin/*.o
	rm bin/*.o

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache
	rm -f ../Tests/premier-jeu-test/*.me ../Tests/premier-jeu-test/*.out
	rm -f ../Tests/Only4G33ks/testFile/*.me ../Tests/Only4G33ks/testFile/*.out
	rm -f ../Tests/testDemo/*.me ../Tests/testDemo/*.out

testsSimple: $(MAIN)
	@python3 tests/test1.py

testsMedium: $(MAIN)
	@python3 tests/test2.py

testsHard: $(MAIN)
	@python3 tests/test3.py