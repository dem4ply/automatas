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
	Estadisticas_t *estad;
	ulong_t i;
	Automata_t automata;
	int tid;
	bool resp;
	int thread_set = 8;

	sscanf(argv[1], "%lu", &param.n_estados);
	sscanf(argv[2], "%lu", &param.estado);
	sscanf(argv[3], "%lu", &param.etapa);

	param.n_pulsos = 2;
	param.max_automatas_estado = 
		pow(param.n_estados, (param.n_estados * param.n_pulsos ) );
	param.max_cadena = 2 * (param.n_estados - 1);


	char file[thread_set][40];
	FILE *f_write[thread_set];
	for (i = 0; i < thread_set; ++i)
	{
		sprintf(file[i], "a-%lu.p-%lu.e-%lu.bin", param.estado, i, param.etapa);
		f_write[i] = fopen(file[i], "w");
	}

	if(param.etapa == 1)
	{
		omp_set_num_threads(thread_set);
		int thread_n = omp_get_num_threads();
		estad = (Estadisticas_t*) calloc(thread_n, sizeof(Estadisticas_t) );

		#pragma omp parallel for schedule(dynamic, 10000) private(automata, i, tid, resp) shared(estad, f_write)
		for(i = 0; i < param.max_automatas_estado; ++i)
		{
			tid = omp_get_thread_num();
			if(tid == 0)
			{
				system("clear");
				ulong_t n_automatas, n_pass;
				n_automatas = 0; n_pass = 0;
				for (i = 0; i < thread_n; ++i)
				{
					n_automatas += estad[i].n_automatas;
					n_pass += estad[i].n_pass;
				}
					printf("n_automatas   = %lu\nmax_automatas = %lu\nn_pass = %lu\n",
							n_automatas,
							param.max_automatas_estado,
							n_pass
							);
			}

			Init_automata(&param, &automata, param.estado, i);

			resp = Compare_Automatas_first(&param, &automata);

			if (resp)
			{
				++estad[tid].n_pass;
				Push_automata_b(&param, &automata, f_write[tid]);
			}
			++estad[tid].n_automatas;
		}
	} //final de etapa 1
	return 0;
}
