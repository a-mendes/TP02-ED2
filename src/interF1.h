#ifndef interF1_h
#define interF1_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/heap.h"

#define TOTALFITA 20
#define TAMFITAINT 19
#define POSFITAEXT 19

void nomeiaArquivo(char nomes[20][20]);
void criaArquivo(FILE *arqvs[20], char nomes[20][20]);
void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam);
void fechaArq(FILE *arqvs[TOTALFITA]);
Alunos readFile(FILE *prova);
void intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam);
void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam);
void imprimeFitaSaida(FILE *arqvs[TOTALFITA]);
void redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
int teste();

#endif