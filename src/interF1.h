#ifndef interF1_h
#define interF1_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long inscricao;
    float nota;
    char estado[2];
    char cidade[50];
    char curso[30];
} Alunos;

typedef struct {
    Alunos aluno;
    bool maior;
} Estrutura;


void nomeiaArquivo(FILE* prova, char nomes[20][20]);
void criaArquivo(FILE* arqvs[20], char nomes[20][20]);
Alunos readFile(FILE *prova);

#endif