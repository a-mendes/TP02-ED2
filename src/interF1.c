#include "interF1.h"
// Obter os arquivos originais ordanados ascendentes, descendentes e aleatoiamente, e apartir deles fazer a ordenação

int main() {
    FILE* prova = fopen("../data/PROVAO.TXT", "r");
    char nomes[20][20] = {""};
    FILE* arqvs[20];
    Estrutura alunosEmMemoria;

    nomeiaArquivo(prova, nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < 20; i++) {
        // alunosEmMemoria.vetAlunos[i] = readFile(prova);
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
        arqvs[i] = fopen(nomes[i], "wb");
    }
}

Alunos readFile(FILE* prova) {
    Alunos aluno;

    fscanf(prova, "%ld %f %s %s %s", &aluno.inscricao, &aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
    
    return aluno;
}