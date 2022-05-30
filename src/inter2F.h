
#ifndef inter2f_h
#define inter2f_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/heap.h"

#define FF_TAMFITAINT 10
#define FF_POSFITAEXT 10

void intercalacao2F(int quantidade, int situacao, int opcional);

void FF_geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam, int quantidade);
int FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual, int fitaSaidaAtual, int tamEntrada, int countNiveis);
int FF_inicializarMemoriaIntercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaInicial);
void imprimirFitas(FILE *arqvs[TOTALFITA]);
void FF_preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int *vetTam);
void FF_imprimeFitaSaida(FILE *arqvs[TOTALFITA]);
void FF_redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
int getFirstCurrentIndex(int currentFita);
int getFirstOppositeIndex(int currentFita);
int getLastCurrentIndex(int currentFita);
int getLastOppositeIndex(int currentFita);
int isInFirstTape(int currentTape);
Alunos getAlunoVazio();

#endif