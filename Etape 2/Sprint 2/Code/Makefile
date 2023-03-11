MAIN := httpParser
SRC_C := $(wildcard */*.c *.c)

$(MAIN): $(SRC_C)
	gcc $^ -o $@

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache
	rm -f ../../Tests/premier-jeu-test/*.me ../../Tests/premier-jeu-test/*.prof
	rm -f ../../Tests/Only4G33ks/testFile/*.me ../../Tests/Only4G33ks/testFile/*.prof
	rm -f Tests/*.me* Tests/*.prof*

tests1: $(MAIN)
	@ python test1.py

tests2: $(MAIN)
	@ python test2.py