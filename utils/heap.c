#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long inscricao;
    float nota;
} Alunos;

typedef struct {
    Alunos aluno;
    bool maior;
} Estrutura;

void HEAP_REFAZ(Estrutura *v, int esq, int dir) {
    int i = esq, z;
    int j = i * 2 + 1;
    Estrutura aux = v[i];

    while (j <= dir) {
        if (j < dir && v[j].aluno.nota > v[j + 1].aluno.nota && v[j + 1].maior != true) {
            j = j + 1;  // j recebe o outro filho de i
        }
        if (aux.aluno.nota <= v[j].aluno.nota && aux.maior != true) {
            break;
        }

        v[i].aluno = v[j].aluno;
        i = j;
        j = i * 2 + 1;
    }
    v[i] = aux;
}

void HEAP_CONSTROI(Estrutura *v, int tam) {
    int esq = (tam / 2) - 1;
    while (esq >= 0) {
        HEAP_REFAZ(v, esq, tam - 1);
        esq--;
    }
}

void Insere_No(Estrutura *v, Alunos elem, int *tam) {
    if (*tam > 19) {
        printf("\nUltrapassou tamanho limite\n");
        return;
    }
    if (*tam == 0) {
        v[*tam].aluno = elem;
        v[*tam].maior = false;
        *tam += 1;
    } else {
        v[*tam].aluno = elem;
        v[*tam].maior = false;
        *tam += 1;
        HEAP_CONSTROI(v, *tam);
    }
}

void remove_No(Estrutura *v, int *tam) {
    if (*tam == 0) {
        return;
    } else {
        v[0].aluno = v[*tam - 1].aluno;
        *tam -= 1;
        HEAP_CONSTROI(v, *tam);
    }
}

void substitui(Estrutura *v, int *tam, Alunos elem) {
    if (*tam == 0) {
        return;
    } else {
        if (v[0].aluno.nota > elem.nota) {
            v[0].aluno = elem;
            v[0].maior = true;
        } else {
            v[0].aluno = elem;
            v[0].maior = false;
        }
    }
    HEAP_CONSTROI(v, *tam);
}

int main() {
    int tam = 0;
    Estrutura vet[20];

    Alunos alun;
    Alunos alun1;
    Alunos alun2;

    alun.nota = 6;
    alun1.nota = 7;
    alun2.nota = 3;

    vet[0].aluno.nota = 3;
    vet[0].maior = false;
    vet[1].aluno.nota = 4;
    vet[1].maior = false;
    vet[2].aluno.nota = 0;
    vet[2].maior = false;
    vet[3].aluno.nota = 6;
    vet[3].maior = false;
    vet[4].aluno.nota = 5;
    vet[4].maior = false;
    vet[5].aluno.nota = 9;
    vet[5].maior = false;
    vet[6].aluno.nota = 3;
    vet[6].maior = false;
    tam = 7;

    for (int i = 0; i < 7; i++) {
        printf("%.0f ", vet[i].aluno.nota);
    }
    printf("\n\n");

    HEAP_CONSTROI(vet, tam);

    printf("\n\n");

    Insere_No(vet, alun, &tam);
    Insere_No(vet, alun1, &tam);
    Insere_No(vet, alun2, &tam);

    printf("\n %d \n", tam);

    for (int i = 0; i < tam; i++) {
        printf("%.0f ", vet[i].aluno.nota);
    }
    printf("\nAQUI\n");

    Alunos alur, alur1;
    alur.nota = 5;
    // alur1.nota = 1;

    substitui(vet, &tam, alur);
    substitui(vet, &tam, alur1);

    for (int i = 0; i < tam; i++) {
        printf("%.0f ", vet[i].aluno.nota);
    }

    printf("\nNaoNulo %d\n", tam);

    return 0;
}