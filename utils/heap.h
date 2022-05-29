#ifndef heap_h
#define heap_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void HEAP_REFAZ(Estrutura *v, int esq, int dir);
void HEAP_CONSTROI(Estrutura *v, int tam);
void Insere_No(Estrutura *v, Alunos elem, int *tam);
bool remove_No(Estrutura *v, int *tam);
bool substitui(Estrutura *v, int *tam, Alunos elem);

#endif