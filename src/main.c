#include "interF1.h"

int main() {
    // FILE* prova = fopen("../data/ProvaoAleatorio.dat", "rb");  // Arquivo de entrada

    FILE *prova = fopen("./data/AleatorioMenor.dat", "rb");  // teste

    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[TOTALFITA];                   // Apontador para as fitas
    Estrutura alunosEmMemoria[TAMFITAINT];    // voltat intam
    int vetTam = 0;                           // Estrutura que guarda os alunos em memória

    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam);

    intercalacao(arqvs, alunosEmMemoria, &vetTam);

    imprimeFitaSaida(arqvs);

    redistribuicao(arqvs, nomes);

    fechaArq(arqvs);
    fclose(prova);

    return 0;
}