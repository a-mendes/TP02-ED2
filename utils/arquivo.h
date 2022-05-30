#ifndef arquivo_h
#define arquivo_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTALFITA 20

typedef struct {
    long inscricao;
    float nota;
    char estado[3];
    char cidade[50];
    char curso[30];
} Alunos;

typedef struct {
    Alunos aluno;
    bool maior;
    int posFita;
} Estrutura;

void fechaArq(FILE *arqvs[TOTALFITA]);
void nomeiaArquivo(char nomes[TOTALFITA][TOTALFITA]);
void criaArquivo(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
Alunos readFile(FILE *prova);
void trocarArquivos(FILE *arqvs[TOTALFITA], int a, int b);

#endif