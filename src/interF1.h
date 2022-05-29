#ifndef interF1_h
#define interF1_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/arquivo.h"
#include "../utils/heap.h"

#define TAMFITAINT 19
#define POSFITAEXT 19


int intercalacaoF1(int quantidade, int situacao, int opcional);

void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam, int quantidade);
int intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam);
void redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);

void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam);
void imprimeFitaSaida(FILE *arqvs[TOTALFITA]);


#endif