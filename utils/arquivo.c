#include "arquivo.h"

// Fecha todos arquivos do vetor de arquivos
void fechaArq(FILE *arqvs[TOTALFITA]) {
    for (int i = 0; i < TOTALFITA; i++) {
        fclose(arqvs[i]);
    }
}
// Cria uma lista de nomes para cada fita enumerando-as
void nomeiaArquivo(char nomes[TOTALFITA][TOTALFITA]) {
    char buffer[3];
    char aux[TOTALFITA] = {"./data/fita"};

    for (int i = 1; i <= TOTALFITA; i++) {
        itoa(i, buffer, 10);
        strcat(aux, buffer);
        strcpy(nomes[i - 1], strcat(aux, ".dat"));
        aux[11] = '\0';
    }
}
// Apre todos as fitas
void criaArquivo(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]) {
    for (int i = 0; i < 20; i++) {
        arqvs[i] = fopen(nomes[i], "wb+");
    }
}
// Faz a leitura de um único elemento(Alunos), de um arquivo passado como parâmetro
Alunos readFile(FILE *prova) {
    Alunos aluno;

    fread(&aluno, sizeof(Alunos), 1, prova);

    return aluno;
}
// Trocc dois arquivos de posição
void trocarArquivos(FILE *arqvs[TOTALFITA], int a, int b) {
    FILE *aux = arqvs[a];
    arqvs[a] = arqvs[b];
    arqvs[b] = aux;
}
// Retorna um ponteiro para o arquivo que deve ser aberto
FILE *escolherArquivoPorSituacao(int situacao) {
    switch (situacao) {
        case 1:
            return fopen("./data/ProvaoAscendente.dat", "rb");

        case 2:
            return fopen("./data/ProvaoDescendente.dat", "rb");

        case 3:
            return fopen("./data/ProvaoAleatorio.dat", "rb");

        default:
            printf("Opcao invalida.");
            exit(EXIT_FAILURE);
    }
}