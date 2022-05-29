
#ifndef inter2f_h
#define inter2f_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/heap.h"

#define FF_TOTALFITA 20
#define FF_TAMFITAINT 9
#define FF_POSFITAEXT 9

void FF_geraBlocos(FILE *arqvs[FF_TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam);
void FF_intercalacao(FILE *arqvs[FF_TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_preencheVetorAlunos(FILE *arqvs[FF_TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_imprimeFitaSaida(FILE *arqvs[FF_TOTALFITA]);
void FF_redistribuicao(FILE *arqvs[FF_TOTALFITA], char nomes[FF_TOTALFITA][FF_TOTALFITA]);
int FF_teste();

#endif