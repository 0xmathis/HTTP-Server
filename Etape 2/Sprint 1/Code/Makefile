MAIN := httpParser
SRC_C := $(wildcard */*.c *.c)

$(MAIN): $(SRC_C)
	gcc $^ -o $@

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache
	rm -f ../../Tests/testgrammar/testFile/*.me* ../../Tests/testgrammar/testFile/*.prof*
	rm -f Tests/*.me* Tests/*.prof*

tests1: $(MAIN)
	@ python test1.py

tests2: $(MAIN)
	@ python test2.py