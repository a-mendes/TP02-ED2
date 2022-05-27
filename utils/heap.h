#ifndef heap_h
#define heap_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/interF1.h"

void HEAP_REFAZ(Estrutura *v, int esq, int dir);
void HEAP_CONSTROI(Estrutura *v, int tam);
void Insere_No(Estrutura *v, Alunos elem, int *tam);
void remove_No(Estrutura *v, int *tam);
bool substitui(Estrutura *v, int *tam, Alunos elem);

#endif