#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef int bool;
typedef unsigned long ulong_t;

typedef struct Param
{
	ulong_t n_estados;
	ulong_t n_pulsos;

	ulong_t n_automatas;
}Param_t;

typedef struct Automata
{
	ulong_t *estados;
	ulong_t **transiciones;

	bool isomorfo;
	ulong_t estado_seed;
	ulong_t transicion_seed;
} Automata_t;

void Init_automata(Param_t *param, Automata_t *automata, ulong_t estado_seed, ulong_t transicion_seed);
void Print_automata(Param_t *param, Automata_t *automata);


void Init_automata(Param_t *param, Automata_t *automata, ulong_t estado_seed, ulong_t transicion_seed)
{
	ulong_t i = sizeof(ulong_t);

	//creacion de estados
	automata->estados = (ulong_t*) calloc(param->n_estados, sizeof(ulong_t) );
	//creacion de tranciciones
	automata->transiciones = (ulong_t**) malloc(param->n_pulsos * sizeof(ulong_t*) );
	for (i = 0; i < param->n_pulsos; ++i)
		automata->transiciones[i] = (ulong_t*) calloc(param->n_estados, sizeof(ulong_t));

	automata->isomorfo = TRUE;
	automata->estado_seed = estado_seed;
	automata->transicion_seed = transicion_seed;
}

void Print_automata(Param_t *param, Automata_t *automata)
{
	ulong i, j;
	for (i = 0; i < param->n_estados; ++i)
	{
		printf("%lu->(", automata->estados[i] );
		for (j = 0; j < param->n_pulsos; ++j)
		{
			printf("%lu, ", automata->transiciones[j][i]);
		}
		printf(")\n");
	}
}
