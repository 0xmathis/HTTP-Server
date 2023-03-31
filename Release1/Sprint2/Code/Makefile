MAIN := httpParser
SRC_C := $(wildcard */*.c *.c)

$(MAIN): $(SRC_C)
	gcc -Wall $^ -o $@

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache
	rm -f ../../Tests/premier-jeu-test/*.me ../../Tests/premier-jeu-test/*.out
	rm -f ../../Tests/Only4G33ks/testFile/*.me ../../Tests/Only4G33ks/testFile/*.out
	rm -f ../../Tests/testDemo/*.me ../../Tests/testDemo/*.out

testsSimple: $(MAIN)
	@ python test1.py

testsMedium: $(MAIN)
	@ python test2.py

testsHard: $(MAIN)
	@ python test3.py