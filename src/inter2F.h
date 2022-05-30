
#ifndef inter2f_h
#define inter2f_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/heap.h"

#define FF_TAMFITAINT 10
#define FF_POSFITAEXT 10

void FF_geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam);
int FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual);
void FF_preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_imprimeFitaSaida(FILE *arqvs[TOTALFITA]);
void FF_redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
int FF_teste();

int getFirstCurrentIndex(int currentFita);
int getFirstOppositeIndex(int currentFita);
int getLastCurrentIndex(int currentFita);
int getLastOppositeIndex(int currentFita);
int isInFirstTape(int currentTape);
Alunos getAlunoVazio();

#endif