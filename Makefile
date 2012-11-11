.PHONY : clean install
automata: automata.c automata.h 
	gcc -Wall -fopenmp $< -o $@ -lm 
