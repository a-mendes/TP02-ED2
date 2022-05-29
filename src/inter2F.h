
#ifndef inter2f_h
#define inter2f_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/heap.h"

#define FF_TAMFITAINT 9
#define FF_POSFITAEXT 9

void FF_geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam);
void FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_imprimeFitaSaida(FILE *arqvs[TOTALFITA]);
void FF_redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
int FF_teste();

#endif