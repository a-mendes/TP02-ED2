#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/interF1.h"

void lerBin();
void tiraEspaco(Alunos *aluno);

int main() {
    FILE *arq = fopen("../data/PROVAO.TXT", "r");
    FILE *binAl = fopen("../data/ProvaoAleatorio.dat", "wb");
    // FILE *binAs = fopen("ProvaoAscendente.dat", "wb");
    // FILE *binDe = fopen("ProvaoDescendente.dat", "wb");

    char aux1[102];
    char aux2[10];
    int i, j, tam;

    Alunos auxAluno;

    while (!feof(arq)) {
        fgets(aux1, 102, arq);
        tam = strlen(aux1);
        aux1[tam - 1] = '\0';

        for (i = 0; i < 8; i++) {
            aux2[i] = aux1[i];
        }
        auxAluno.inscricao = atoi(aux2);
        i++;
        for (j = 0; j < 5; j++, i++) {
            aux2[j] = aux1[i];
        }
        aux2[j] = '\0';
        auxAluno.nota = atof(aux2);
        i++;
        for (j = 0; j < 2; j++, i++) {
            auxAluno.estado[j] = aux1[i];
        }
        i++;
        for (j = 0; j < 50; j++, i++) {
            auxAluno.cidade[j] = aux1[i];
        }
        i++;
        for (j = 0; j < 30; j++, i++) {
            auxAluno.curso[j] = aux1[i];
        }

        tiraEspaco(&auxAluno);

        fwrite(&auxAluno, sizeof(Alunos), 1, binAl);
        // fwrite(&auxAluno, sizeof(Alunos), 1, binAs);
        // fwrite(&auxAluno, sizeof(Alunos), 1, binDe);
    }

    fclose(arq);
    fclose(binAl);
    // fclose(binAs);
    // fclose(binDe);

    // lerBin();

    return 0;
}

void lerBin() {
    FILE *bin = fopen("ProvaoAleatorio.dat", "rb");
    Alunos teste;
    int cont =0;
    while(!feof(bin)) {
        fread(&teste, sizeof(Alunos), 1, bin);

        printf("%d -",cont++);
        printf("%ld ", teste.inscricao);
        printf("%.2f ", teste.nota);
        printf("%s ", teste.estado);
        printf("%s ", teste.cidade);
        printf("%s \n", teste.curso);
    }

    // printf("%s %d \n", teste.curso, (int)strlen(teste.curso));
}

void tiraEspaco(Alunos *itemAluno) {
    itemAluno->estado[2] = '\0';
    
    for(int i = 0; i < 50; i++) {
        if(itemAluno->cidade[i] == ' ' && itemAluno->cidade[i + 1] == ' ') {
            itemAluno->cidade[i] = '\0';
            break;
        }
    }
    for(int i = 0; i < 30; i++) {
        if(itemAluno->curso[i] == ' ' && itemAluno->curso[i + 1] == ' ') {
            itemAluno->curso[i] = '\0';
            break;
        }
    }
}