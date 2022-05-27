#include "interF1.h"
// Obter os arquivos originais ordanados ascendentes, descendentes e aleatoiamente, e apartir deles fazer a ordenação

int main() {
    FILE* prova = fopen("../data/PROVAO_ALEATORIO.dat", "rb");  // Arquivo de entrada
    char nomes[20][20] = {""};                                  // Vetor de nomes para criar as fitas
    FILE* arqvs[20];                                            // Apontador para as fitas
    Estrutura alunosEmMemoria[20];                              // Estrutura que guarda os alunos em memória

    nomeiaArquivo(prova, nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < 1; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;

        printf("%ld\n", alunosEmMemoria[i].aluno.inscricao);
        printf("%f\n", alunosEmMemoria[i].aluno.nota);
        printf("%s\n", alunosEmMemoria[i].aluno.estado);
        printf("%s\n", alunosEmMemoria[i].aluno.cidade);
        printf("%s\n", alunosEmMemoria[i].aluno.curso);
        // Insere heap readFile(prova);
    }

    return 0;
}

void nomeiaArquivo(FILE* prova, char nomes[20][20]) {
    char buffer[3];
    char aux[20] = {"../data/fita"};

    for (int i = 1; i <= 20; i++) {
        itoa(i, buffer, 10);
        strcat(aux, buffer);
        strcpy(nomes[i - 1], strcat(aux, ".dat"));
        aux[12] = '\0';
    }
}

void criaArquivo(FILE* arqvs[20], char nomes[20][20]) {
    for (int i = 0; i < 20; i++) {
        arqvs[i] = fopen(nomes[i], "wb+");
    }
}

Alunos readFile(FILE* prova) {
    Alunos aluno;

    fread(&aluno, sizeof(Alunos), 1, prova);

    return aluno;
}