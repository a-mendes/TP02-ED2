#include "inter2F.h"

#include "../utils/arquivo.h"

int FF_teste() {
    FILE *prova = fopen("./data/AleatorioMenor.dat", "rb");  // teste

    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[FF_TOTALFITA];                // Apontador para as fitas
    Estrutura alunosEmMemoria[FF_TAMFITAINT];
    int vetTam = 0;  // Estrutura que guarda os alunos em memória

    printf("jair");
    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < FF_TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    fechaArq(arqvs);
    fclose(prova);
    return 0;
}
