#include <math.h>
#include <omp.h>
#include <unistd.h>
#include "automata.h"

typedef struct Estadisticas
{
	ulong_t n_automatas;
	ulong_t n_pass;
	bool is_eof;
} Estadisticas_t;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("parametros: estados estado etapa\n");
		return 0;
	}
	Param_t param;
	ulong_t i;
	Automata_t a1;

	sscanf(argv[1], "%lu", &param.n_estados);
	//sscanf(argv[2], "%lu", &param.estados);
	sscanf(argv[3], "%lu", &param.etapa);

	param.n_pulsos = 2;
	param.max_automatas_estado = 
		pow(param.n_estados, (param.n_estados * param.n_pulsos ) );
	param.max_cadena = 2 * (param.n_estados - 1);

	Init_automata(&param, &a1, 0, 0);
	FILE *f = fopen("_a-7.e-2.bin", "r");
	char *s1;

	for (i = 0; !feof(f); ++i)
	{
		Pull_automata_b(&param, &a1, f);
		Automata_to_string(&param, &a1, &s1);
		printf("%s\n", s1);
	}
	return 0;
}
