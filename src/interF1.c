#include "interF1.h"

#include "../utils/arquivo.h"
#include "../utils/heap.h"

void imprime(Estrutura *v, int tam) {
    int cont = 0;
    printf("\n");
    for (int i = 0; i < tam; i++) {
        printf("%d - %ld %.1f %s %s %s T/F: %d \n", cont++, v[i].aluno.inscricao, v[i].aluno.nota, v[i].aluno.estado, v[i].aluno.cidade, v[i].aluno.curso, v[i].maior);
    }
    printf("\n");
}


int intercalacaoF1(int quantidade, int situacao, int opcional) {
    FILE *prova;
    // FILE *prova = fopen("./data/AleatorioMenor.dat", "rb");  // teste
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[TOTALFITA];                   // Apontador para as fitas
    Estrutura alunosEmMemoria[TAMFITAINT];    // voltat intam;
    int vetTam = 0;                           // Estrutura que guarda os alunos em memória

    switch (situacao) {
        case 1:
            //prova = fopen("./data/ProvaoAscendente.dat", "rb");
            break;
        case 2:
            //prova = fopen("./data/ProvaoDescendente.dat", "rb");
            break;
        case 3:
            prova = fopen("./data/ProvaoAleatorio.dat", "rb");
            break;

        default:
            printf("Tu é um animal que não sabe contar\n");
            break;
    }

    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);

    while(intercalacao(arqvs, alunosEmMemoria, &vetTam)) { 
        redistribuicao(arqvs, nomes);
    }

    if(opcional)
        imprimeFitaSaida(arqvs);

    fechaArq(arqvs);
    fclose(prova);

    return 0;
}

void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int numfita = 0;
    int cont = 17;
    Alunos alunoNulo;
    alunoNulo.nota = -1;

    while (*vetTam > 1) {
        cont++;
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[numfita]);

        if (feof(prova) || cont >= quantidade) {
            if (remove_No(alunosEmMemoria, vetTam) || *vetTam == 1) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                if(*vetTam != 1)
                    numfita += 1;
            }
        } else if (substitui(alunosEmMemoria, vetTam, readFile(prova))) {
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
            numfita += 1;
        }

        if (numfita >= TAMFITAINT)
            numfita = 0;
    }

    *vetTam = numfita + 1;

    Alunos teste; ///////////////////////////////// PARA TESTE // APAGAR
    cont = 0;
    for (int i = 0; i < 19; i++) {
        rewind(arqvs[i]);
        printf("FITA %d \n", i + 1);
        while (fread(&teste, sizeof(Alunos), 1, arqvs[i])) {
            printf("%d -", cont++);
            printf("%ld ", teste.inscricao);
            printf("%.2f ", teste.nota);
            printf("%s ", teste.estado);
            printf("%s ", teste.cidade);
            printf("%s \n", teste.curso);
        }
        printf("\n\n\n");
    }
}

int intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    int blocosFitaExt = 0;
    Alunos aluno;
    Alunos alunoNulo;
    alunoNulo.nota = -1;

    preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);

    while (*vetTam > 0) {
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[POSFITAEXT]);

        if (!fread(&aluno, sizeof(Alunos), 1, arqvs[alunosEmMemoria[0].posFita]))
            aluno.nota = -1;

        if (aluno.nota == -1)
            remove_No(alunosEmMemoria, vetTam);
        else
            substitui(alunosEmMemoria, vetTam, aluno);

        if (*vetTam == 0) {
            blocosFitaExt++;
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
            preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);
        }
    }

    if(blocosFitaExt > 1)
        return 1;
    
    return 0;
}

void redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]) {
    // Reseta os arquivos
    for (int i = 0; i < 19; i++) {
        arqvs[i] = freopen(nomes[i], "wb", arqvs[i]);
    }
    for(int i = 0; i < TOTALFITA; i++ ) {
        rewind(arqvs[i]);
    }

    Alunos aux;  // Aluno auxiliar para ler os arquivos

    printf("REDISTRIBUICAO\n\n\n\n");

    int contador = 0;
    while (fread(&aux, sizeof(Alunos), 1, arqvs[POSFITAEXT])) {  // le o ultimo arquivo

        if (aux.nota == -1) {  // se for -1, passa pra proxima fita
            fwrite(&aux, sizeof(Alunos), 1, arqvs[contador++]);
            fread(&aux, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
            if (contador == 19) {
                contador = 0;
            }
        }
        fwrite(&aux, sizeof(Alunos), 1, arqvs[contador]);     // escreve no arquivo correto
    }

    //rewind para o inicio dos arquivos
    for (int i = 0; i < TOTALFITA; i++) {
        rewind(arqvs[i]);
    }


    Alunos teste; ///////////////////////////////// PARA TESTE // APAGAR
    int cont = 0;
    for (int i = 0; i < 19; i++) {
        rewind(arqvs[i]);
        printf("FITA %d \n", i + 1);
        while (fread(&teste, sizeof(Alunos), 1, arqvs[i])) {
            printf("%d -", cont++);
            printf("%ld ", teste.inscricao);
            printf("%.2f ", teste.nota);
            printf("%s ", teste.estado);
            printf("%s ", teste.cidade);
            printf("%s \n", teste.curso);
        }
        printf("\n\n\n");
    }
    printf("\n\n\n");

}


void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    Alunos aluno;
    if (*vetTam != 0) {
        for (int i = 0; i <= *vetTam; i++) {
            rewind(arqvs[i]);
            fread(&alunosEmMemoria[i].aluno, sizeof(Alunos), 1, arqvs[i]);
            alunosEmMemoria[i].posFita = i;
            alunosEmMemoria[i].maior = false;
        }
    } else {
        for (int j = 0, i = 0; i < TAMFITAINT; i++) {
            if (fread(&alunosEmMemoria[j].aluno, sizeof(Alunos), 1, arqvs[i])) {
                alunosEmMemoria[j].aluno = aluno;
                alunosEmMemoria[j].posFita = i;
                alunosEmMemoria[j++].maior = false;
                *vetTam += 1;
            }
        }
    }

    HEAP_CONSTROI(alunosEmMemoria, *vetTam);
}

void imprimeFitaSaida(FILE *arqvs[TOTALFITA]) {
    Alunos aluno;
    int cont = 0;
    rewind(arqvs[POSFITAEXT]);
    printf("\n\nFITA DE SAIDA\n");

    while (fread(&aluno, sizeof(Alunos), 1, arqvs[POSFITAEXT])) {
        printf("%d -", cont++);
        printf("%ld ", aluno.inscricao);
        printf("%.2f ", aluno.nota);
        printf("%s ", aluno.estado);
        printf("%s ", aluno.cidade);
        printf("%s \n", aluno.curso);
    }
    printf("\n\n");
}
