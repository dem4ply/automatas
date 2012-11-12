#include <math.h>
#include <omp.h>
#include <unistd.h>
#include "automata.h"

typedef struct Estadisticas
{
	ulong_t n_automatas;
	ulong_t n_pass;
	ulong_t q;
	ulong_t c;
} Estadisticas_t;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("parametros: estados 'max num de automatas por archivo'\n");
		return 0;
	}
	Param_t param;
	Estadisticas_t *estad;
	ulong_t i, j;
	Automata_t automata;

	sscanf(argv[1], "%lu", &param.n_estados);
	sscanf(argv[2], "%lu", &param.max_archivo);

	param.n_pulsos = 2;
	param.max_automatas_estado = 9765624;
	param.max_cadena = pow(2, (param.n_estados - 1) ) * 2 + 1;

	param.estados = (ulong*) malloc( (param.n_estados - 1) * sizeof(ulong_t) );
	for (i = 0, j = 0; i < (param.n_estados - 1); ++i)
		param.estados[i] = j = (j << 1) + 1;
	
	estad = (Estadisticas_t*) calloc(param.n_estados -1, sizeof(Estadisticas_t) );

	omp_set_num_threads(param.n_estados);

	#pragma omp parallel private(automata) shared(estad)
	{
		#pragma omp master
		{
			int i;
			while (1)
			{
				for (i = 0; i < param.n_estados-1; ++i)
				{
					printf("thread = %d\nn_automatas = %lu\nn_pass = %lu\n", i, estad[i].n_automatas, estad[i].n_pass);
					printf("q = %lu\nc = %lu\n\n", estad[i].q, estad[i].c);
				}
				sleep(1);
				system("clear");
			}
		}
		int thread = omp_get_thread_num();
		if (thread)
		{
			char file[15];
			sprintf(file, "a-%lu.e-1.bin", param.estados[thread - 1]);
			int i, q, c, max_c;
			bool pass = FALSE;
			FILE *f = fopen(file, "w");
			for (i = 0; i < param.max_automatas_estado; ++i)
			{
				++estad[thread - 1].n_automatas;
				Init_automata(&param, &automata, param.estados[thread - 1], i);
				pass = FALSE;
				for (q = 0; q < param.max_cadena && !pass; ++q)
				{
					estad[thread - 1].q = q;
					max_c = pow(param.n_pulsos, q);
					for (c = 0; c < max_c && !pass; ++c)
					{
						estad[thread - 1].c = c;
						if (automata.estados[0] != Get_answer(&param, &automata, c, q) )
							pass = TRUE;
					}
				}

				if (pass)
				{
					Push_automata_b(&param, &automata, f);
					++estad[thread - 1].n_pass;
				}

			}
		}
	}


	return 0;
}
