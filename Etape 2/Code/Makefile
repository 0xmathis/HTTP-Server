MAIN := httpParser
SRC_C := $(wildcard */*.c *.c)

$(MAIN): $(SRC_C)
	gcc $^ -o $@

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache

tests: $(MAIN)
	@ python test.py