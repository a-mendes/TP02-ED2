#include "interF1.h"

#include "../utils/arquivo.h"
#include "../utils/heap.h"

int numLeitura = 0;
int numEscrita = 0;
int numComparacoes = 0;
clock_t tempoInicial;
clock_t tempoFinal;

/// COMENTAR E MOSTRAR AS ANALISES ///////////////////////////////////////////
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int intercalacaoF1(int quantidade, int situacao, int opcional) {
    FILE *prova;
    // FILE *prova = fopen("./data/AleatorioMenor.dat", "rb");  // teste
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[TOTALFITA];                   // Apontador para as fitas
    Estrutura alunosEmMemoria[TAMFITAINT];    // voltat intam;
    int vetTam = 0;                           // Estrutura que guarda os alunos em memória

    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);

    while (intercalacao(arqvs, alunosEmMemoria, &vetTam)) {
        redistribuicao(arqvs, nomes, &vetTam);
    }

    exibirResultados(opcional, arqvs);

    fechaArq(arqvs);
    fclose(prova);

    return 0;
}

void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int numfita = 0;
    int cont = 17, segundoBloco = 0;
    Alunos alunoNulo;
    alunoNulo.nota = -1;

    while (*vetTam > 1) {
        cont++;
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[numfita]);

        if (feof(prova) || cont >= quantidade) {
            if (remove_No(alunosEmMemoria, vetTam) || *vetTam == 1) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                if (*vetTam != 1)
                    numfita += 1;
            }
        } else if (substitui(alunosEmMemoria, vetTam, readFile(prova))) {
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
            numfita += 1;
        }

        if (numfita >= TAMFITAINT) {
            numfita = 0;
            segundoBloco = 1;
        }
    }

    *vetTam = segundoBloco;

    // Alunos teste; ///////////////////////////////// PARA TESTE // APAGAR
    // cont = 0;
    // for (int i = 0; i < 19; i++) {
    //     rewind(arqvs[i]);
    //     printf("FITA BLOCO %d \n", i + 1);
    //     while (fread(&teste, sizeof(Alunos), 1, arqvs[i])) {
    //         printf("%d -", cont++);
    //         printf("%ld ", teste.inscricao);
    //         printf("%.2f ", teste.nota);
    //         printf("%s ", teste.estado);
    //         printf("%s ", teste.cidade);
    //         printf("%s \n", teste.curso);
    //     }
    //     printf("\n\n\n");
    // }
}

int intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    int blocosFitaSaida = 0;
    Alunos aluno;
    Alunos alunoNulo;
    alunoNulo.nota = -1;
    int segundoBloco = *vetTam;

    for (int i = 0; i < TOTALFITA; i++) {
        rewind(arqvs[i]);
    }

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
            blocosFitaSaida++;
            if (segundoBloco)
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
            preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);
        }
    }

    return blocosFitaSaida > 1 ? 1 : 0;
}

void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    *vetTam = 0;

    for (int j = 0, i = 0; i < TAMFITAINT; i++) {
        if (fread(&alunosEmMemoria[j].aluno, sizeof(Alunos), 1, arqvs[i])) {
            alunosEmMemoria[j].posFita = i;
            alunosEmMemoria[j++].maior = false;
            *vetTam += 1;
        }
    }
    HEAP_CONSTROI(alunosEmMemoria, *vetTam);
}

void redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA], int *vetTam) {
    Alunos aux;
    int cont = 0;
    *vetTam = 0;

    for (int i = 0; i < TAMFITAINT; i++) {
        arqvs[i] = freopen(nomes[i], "wb", arqvs[i]);
        arqvs[i] = freopen(nomes[i], "wb+", arqvs[i]);
    }
    rewind(arqvs[POSFITAEXT]);

    while (fread(&aux, sizeof(Alunos), 1, arqvs[POSFITAEXT])) {
        if (aux.nota != -1)
            fwrite(&aux, sizeof(Alunos), 1, arqvs[cont]);
        else {
            fwrite(&aux, sizeof(Alunos), 1, arqvs[cont++]);
            if (cont == TAMFITAINT) {
                cont = 0;
                *vetTam = 1;
            }
        }
    }

    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb", arqvs[POSFITAEXT]);
    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb+", arqvs[POSFITAEXT]);

    // rewind para o inicio dos arquivos se FOR PRINTAR
    //  for (int i = 0; i < TOTALFITA; i++) {
    //      rewind(arqvs[i]);
    //  }

    // cont = 0;
    // for (int i = 0; i < 19; i++) {
    //     printf("FITA REDIST %d \n", i + 1);
    //     while (fread(&aux, sizeof(Alunos), 1, arqvs[i])) {
    //         printf("%d -", cont++);
    //         printf("%ld ", aux.inscricao);
    //         printf("%.2f ", aux.nota);
    //         printf("%s ", aux.estado);
    //         printf("%s ", aux.cidade);
    //         printf("%s \n", aux.curso);
    //     }
    //     printf("\n\n\n");
    // }
    // printf("\n\n\n");
}

void imprimeFitaSaida(FILE *arqvs[TOTALFITA]) {
    Alunos aluno;
    int cont = 1;
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

void exibirResultados(int opcional, FILE *arqvs[TOTALFITA]) {
    if (opcional) {
        imprimeFitaSaida(arqvs);
    }

    /**
     * Tempo de execução em segundos
     */

    double tempoExecucao = ((double)tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    printf("\n___________________________________");
    printf("\n            Resultados             ");
    printf("\n-----------------------------------");
    printf("\n# Tempo  de Execucao: %lf seg  ", tempoExecucao);
    printf("\n# Numero de Leituras: %d      ", numLeitura);
    printf("\n# Numero de Escritas: %d      ", numEscrita);
    printf("\n# Numero de Comparacoes: %d   ", numComparacoes);
    printf("\n __________________________________");
}