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
	if (argc != 3)
	{
		printf("parametros: estados etapa\n");
		return 0;
	}
	Param_t param;
	Estadisticas_t *estad;
	ulong_t i, j;
	Automata_t automata;
	int tid;
	bool resp;

	sscanf(argv[1], "%lu", &param.n_estados);
	sscanf(argv[2], "%lu", &param.etapa);

	int thread_set = param.n_estados;

	param.n_pulsos = 2;
	param.max_automatas_estado = 
		pow(param.n_estados, (param.n_estados * param.n_pulsos ) );
	param.max_cadena = 2 * (param.n_estados - 1);

	param.estados = (ulong*) malloc( (param.n_estados - 1) * sizeof(ulong_t) );
	for (i = 0, j = 0; i < (param.n_estados - 1); ++i)
	{
		param.estados[i] = j = (j << 1) + 1;
	}

	if(param.etapa == 1)
	{
		omp_set_num_threads(thread_set);
		estad = (Estadisticas_t*) calloc(thread_set, sizeof(Estadisticas_t) );

		char file[thread_set][40];
		FILE *f_write[param.n_estados];
		for (i = 0; i < (param.n_estados); ++i)
		{
			ulong temp1 = param.estados[i], temp2 = param.etapa;
			sprintf(file[i], "_a-%lu.e-%lu.bin", temp1, temp2);
			f_write[i] = fopen(file[i], "w");
		printf("i: %lu\n", i);
		printf("2i: %lu\n", param.n_estados - 1);
		printf("estado: %lu\n", param.estados[i]);
		}

		#pragma omp parallel private(automata, i, tid, resp) shared(estad, f_write)
		{
			tid = omp_get_thread_num();

			#pragma omp master
			{
				int thread_n = omp_get_num_threads();
				ulong_t total_automatas, 
						  n_automatas = param.max_automatas_estado * (param.n_estados - 1);
				do
				{
					system("clear");
					printf("thread total = %d\n", thread_n);
					printf("thread set = %d\n", thread_set);
					printf("total automatas = %lu\n", total_automatas);
					printf("n_automatas     = %lu\n\n", n_automatas);
					total_automatas = 0;
					for (i = 0; i < thread_n - 1; ++i)
					{
						printf("thread id    = %lu\n", i);
						printf("n_automatas   = %lu\nmax_automatas = %lu\nn_pass = %lu\n\n",
								estad[i].n_automatas,
								param.max_automatas_estado,
								estad[i].n_pass
								);
						total_automatas += estad[i].n_automatas;
					}
					sleep(2);
				}while(total_automatas != n_automatas);
			}
			if (tid)
			{
				--tid;
				for(i = 0; i < param.max_automatas_estado; ++i)
				{

					Init_automata(&param, &automata, param.estados[tid], i);

					resp = Compare_Automatas_first(&param, &automata);

					if (!resp)
					{
						++estad[tid].n_pass;
						//printf("\ntid : %d\n",tid);
						Push_automata_b(&param, &automata, f_write[tid]);
					}
					++estad[tid].n_automatas;
				}
			}
		}
	} //final de etapa 1
	return 0;
}
