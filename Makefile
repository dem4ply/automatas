.PHONY : clean install
automata: automata.c automata.h 
	gcc -Wall -g -fopenmp $< -o $@ -lm 
