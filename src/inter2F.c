#include "inter2F.h"

int FF_teste() {
    FILE *prova = fopen("./data/AleatorioMenor.dat", "rb");  // teste

    char nomes[FF_TOTALFITA][FF_TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[FF_TOTALFITA];                      // Apontador para as fitas
    Estrutura alunosEmMemoria[FF_TAMFITAINT];
    int vetTam = 0;  // Estrutura que guarda os alunos em memória

    printf("jair");
    FF_nomeiaArquivo(nomes);
    FF_criaArquivo(arqvs, nomes);

    FF_fechaArq(arqvs);
    fclose(prova);
    return 0;
}

void FF_fechaArq(FILE *arqvs[FF_TOTALFITA]) {
    for (int i = 0; i < FF_TOTALFITA; i++) {
        fclose(arqvs[i]);
    }
}

void FF_nomeiaArquivo(char nomes[FF_TOTALFITA][FF_TOTALFITA]) {
    char buffer[3];
    char aux[FF_TOTALFITA] = {"./data/fita"};

    for (int i = 1; i <= FF_TOTALFITA; i++) {
        itoa(i, buffer, 10);
        strcat(aux, buffer);
        strcpy(nomes[i - 1], strcat(aux, ".dat"));
        aux[11] = '\0';
    }
}

void FF_criaArquivo(FILE *arqvs[FF_TOTALFITA], char nomes[FF_TOTALFITA][FF_TOTALFITA]) {
    for (int i = 0; i < 20; i++) {
        arqvs[i] = fopen(nomes[i], "wb+");
    }
}
