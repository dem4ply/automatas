#include "automata.h"


int main(int argc, char* argv[])
{
	Param_t param;
	param.n_estados = 5;
	param.n_pulsos = 2;

	Automata_t automata;

	Init_automata(&param, &automata, 3, 6);

	Print_automata(&param, &automata);

	return 0;
}
