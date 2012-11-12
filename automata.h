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
void Init_automata_garbage(Param_t *param, Automata_t *automata);
void Del_automata(Param_t *param, Automata_t *automata);

void Print_automata(Param_t *param, Automata_t *automata);
void Automata_to_string(Param_t *param, Automata_t *automata, char** c);
void Push_automata_b(Param_t *param, Automata_t *automata, FILE *f);
void Pull_automata_b(Param_t *param, Automata_t *automata, FILE *f);



void Init_automata(Param_t *param, Automata_t *automata, ulong_t estado_seed, ulong_t transicion_seed)
{
	ulong_t i, j;

	//creacion de estados
	automata->estados = (ulong_t*) calloc(param->n_estados, sizeof(ulong_t) );
	//creacion de tranciciones
	automata->transiciones = (ulong_t**) malloc(param->n_pulsos * sizeof(ulong_t*) );
	for (i = 0; i < param->n_pulsos; ++i)
		automata->transiciones[i] = (ulong_t*) calloc(param->n_estados, sizeof(ulong_t));

	automata->isomorfo = TRUE;
	automata->estado_seed = estado_seed;
	automata->transicion_seed = transicion_seed;

	for (i = 0; i < param->n_estados; ++i)
	{
		automata->estados[i] = estado_seed & 1;
		estado_seed >>= 1;
	}

	double base_d = 1.0 / param->n_estados;

	for (i = 0; i < param->n_estados; ++i)
		for (j = 0; j < param->n_pulsos; ++j)
		{
			automata->transiciones[j][i] = transicion_seed % param->n_estados;
			transicion_seed *= base_d;
		}
}

void Init_automata_garbage(Param_t *param, Automata_t *automata)
{
	ulong_t i;

	//creacion de estados
	automata->estados = (ulong_t*) malloc(param->n_estados * sizeof(ulong_t) );
	//creacion de tranciciones
	automata->transiciones = (ulong_t**) malloc(param->n_pulsos * sizeof(ulong_t*) );
	for (i = 0; i < param->n_pulsos; ++i)
		automata->transiciones[i] = (ulong_t*) malloc(param->n_estados * sizeof(ulong_t));

}
void Del_automata(Param_t *param, Automata_t *automata)
{
	ulong_t i;
	free(automata->estados);
	for (i = 0; i < param->n_pulsos; ++i)
		free(automata->transiciones[i]);
	free(automata->transiciones);
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

void Automata_to_string(Param_t *param, Automata_t *automata, char** c)
{
	*c = (char*) calloc(128, sizeof(char));
	ulong i, j;
	for (i = 0; i < param->n_estados; ++i)
	{
		sprintf(*c, "%s%lu->(", *c, automata->estados[i] );
		for (j = 0; j < param->n_pulsos; ++j)
		{
			sprintf(*c, "%s%lu", *c, automata->transiciones[j][i]);
			if (j + 1 < param->n_pulsos)
				sprintf(*c, "%s, ", *c);
		}
		sprintf(*c, "%s);", *c);
	}
}

void Push_automata_b(Param_t *param, Automata_t *automata, FILE *f)
{
	ulong_t i;
	fwrite(&automata->isomorfo, sizeof(bool), 1, f);
	fwrite(&automata->estado_seed, sizeof(ulong_t), 1, f);
	fwrite(&automata->transicion_seed, sizeof(ulong_t), 1, f);
	
	fwrite(automata->estados, sizeof(ulong_t), param->n_estados, f);
	for (i = 0; i < param->n_pulsos; ++i)
		fwrite(automata->transiciones[i], sizeof(ulong_t), param->n_estados, f);
}

void Pull_automata_b(Param_t *param, Automata_t *automata, FILE *f)
{

	ulong_t i;
	fread(&automata->isomorfo, sizeof(bool), 1, f);
	fread(&automata->estado_seed, sizeof(ulong_t), 1, f);
	fread(&automata->transicion_seed, sizeof(ulong_t), 1, f);
	
	fread(automata->estados, sizeof(ulong_t), param->n_estados, f);
	for (i = 0; i < param->n_pulsos; ++i)
		fread(automata->transiciones[i], sizeof(ulong_t), param->n_estados, f);
}
