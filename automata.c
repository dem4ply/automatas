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
	bool *is_eof;
} Estadisticas_t;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("parametros: estados 'max num de automatas por archivo' etapa\n");
		return 0;
	}
	Param_t param;
	Estadisticas_t *estad;
	ulong_t i, j;
	Automata_t automata;

	sscanf(argv[1], "%lu", &param.n_estados);
	sscanf(argv[2], "%lu", &param.max_archivo);
	sscanf(argv[3], "%lu", &param.etapa);

	param.n_pulsos = 2;
	param.max_automatas_estado = 
		pow(param.n_estados, (param.n_estados * param.n_pulsos ) );
	param.max_cadena = 2 * (param.n_estados - 1);

	param.estados = (ulong*) malloc( (param.n_estados - 1) * sizeof(ulong_t) );
	for (i = 0, j = 0; i < (param.n_estados - 1); ++i)
		param.estados[i] = j = (j << 1) + 1;
	
	estad = (Estadisticas_t*) calloc(param.n_estados -1, sizeof(Estadisticas_t) );

	if(etapa == 1)
	{

		omp_set_num_threads(param.n_estados);

		#pragma omp parallel private(automata) shared(estad)
		{
			#pragma omp master
			{
				int i, final = 0;
				while (final != param.max_automatas_estado * (param.n_estados - 1 ) )
				{
					system("clear");
					final = 0;
					for (i = 0; i < param.n_estados-1; ++i)
					{
						printf("thread = %d\nn_automatas   = %lu\nmax_automatas = %lu\nn_pass = %lu\n", i, estad[i].n_automatas, param.max_automatas_estado, estad[i].n_pass);
						printf("q = %lu\nc = %lu\n", estad[i].q, estad[i].c);
						final += estad[i].n_automatas;
					}
					printf("final: %lu\ntotal: %lu", final, param.max_automatas_estado * (param.n_estados - 1 ));
					sleep(1);
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
	}
	else if (etapa > 1)
	{
		omp_set_num_threads(param.n_estados - (etapa - 1) );

		#pragma omp parallel private(automata) shared(estad)
		{
			#pragma omp master
			{
				int i, threads = omp_get_threads_num(), kill = FALSE;
				while (kill)
				{
					system("clear");
					final = 0;
					for (i = 0; i < param.n_estados-1; ++i)
					{
						printf("
								thread = %d\n
								n_automatas   = %lu\n
								max_automatas = %lu\n
								n_pass = %lu\n",
								i, estad[i].n_automatas,
								param.max_automatas_estado, estad[i].n_pass);
						printf("q = %lu\nc = %lu\n", estad[i].q, estad[i].c);
					}

					kill = TRUE;
					for (i = 0; i < threads; ++i)
						if (!estad.is_eof[i])
						{
							kill = TRUE;
							break;
						}
					sleep(1);
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
	}
	return 0;
}
