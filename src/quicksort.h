#ifndef quicksort_h
#define quicksort_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <float.h>

#include "../utils/arquivo.h"
#include "../utils/heap.h"

/**
 * Para o método 3, deve ser considerada a existência de memória 
 * interna disponível para armazenar um vetor de, no máximo, 10 registros.
 */

#define TAM_MEMORIA_INTERNA 10
#define TAM_TOTAL_REGISTROS 471705

typedef Alunos TipoArea;

void quicksort(int quantidade, int situacao, int opcional);

#endif