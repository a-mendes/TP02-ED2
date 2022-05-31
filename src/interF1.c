#include "interF1.h"

#include "../utils/arquivo.h"
#include "../utils/heap.h"

Analise analise;

/// COMENTAR E MOSTRAR AS ANALISES ///////////////////////////////////////////
////////////////////////////////////////////////////////


int intercalacaoF1(int quantidade, int situacao, int opcional) {
    FILE *prova;
    analise.numComparacoes = 0;
    analise.numEscrita = 0;
    analise.numLeitura = 0;
    
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    FILE *arqvs[TOTALFITA];                   // Apontador para as fitas
    Estrutura alunosEmMemoria[TAMFITAINT];    // voltat intam;
    int vetTam = 0;                           // Estrutura que guarda os alunos em memória

    prova = escolherArquivoPorSituacao(situacao);

    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    analise.tempoInicial = clock();
    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        analise.numLeitura += 1;
        vetTam++;
    }

    analise.numComparacoes += HEAP_CONSTROI(alunosEmMemoria, vetTam);
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);

    while (intercalacao(arqvs, alunosEmMemoria, &vetTam)) {
        redistribuicao(arqvs, nomes, &vetTam);
    }
    analise.tempoFinal = clock();

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
        analise.numEscrita += 1;

        if (feof(prova) || cont >= quantidade) {
            if (remove_No(alunosEmMemoria, vetTam, &analise) || *vetTam == 1) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                analise.numEscrita += 1;
                if (*vetTam != 1)
                    numfita += 1;
            }
        }
        else {
            analise.numLeitura += 1;
            if(substitui(alunosEmMemoria, vetTam, readFile(prova), &analise)) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                analise.numEscrita += 1;
                numfita += 1;
            }
        }

        if (numfita >= TAMFITAINT) {
            numfita = 0;
            segundoBloco = 1;
        }
    }

    *vetTam = segundoBloco;
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
        analise.numEscrita += 1;

        if (!fread(&aluno, sizeof(Alunos), 1, arqvs[alunosEmMemoria[0].posFita]))
            aluno.nota = -1;

        analise.numLeitura += 1;

        if (aluno.nota == -1)
            remove_No(alunosEmMemoria, vetTam, &analise);
        else
            substitui(alunosEmMemoria, vetTam, aluno, &analise);

        if (*vetTam == 0) {
            blocosFitaSaida++;
            if (segundoBloco)
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
                analise.numEscrita += 1;
            preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);
        }
    }

    return blocosFitaSaida > 1 ? 1 : 0;
}

void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    *vetTam = 0;

    for (int j = 0, i = 0; i < TAMFITAINT; i++) {
        analise.numLeitura += 1;
        if (fread(&alunosEmMemoria[j].aluno, sizeof(Alunos), 1, arqvs[i])) {
            alunosEmMemoria[j].posFita = i;
            alunosEmMemoria[j++].maior = false;
            *vetTam += 1;
        }
    }
    analise.numComparacoes += HEAP_CONSTROI(alunosEmMemoria, *vetTam);
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
        analise.numLeitura += 1;

        if (aux.nota != -1) {
            fwrite(&aux, sizeof(Alunos), 1, arqvs[cont]);
            analise.numEscrita += 1;
        }
        else {
            fwrite(&aux, sizeof(Alunos), 1, arqvs[cont++]);
            analise.numEscrita += 1;
            if (cont == TAMFITAINT) {
                cont = 0;
                *vetTam = 1;
            }
        }
    }

    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb", arqvs[POSFITAEXT]);
    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb+", arqvs[POSFITAEXT]);
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

	double tempoExecucao = ((double)analise.tempoFinal - analise.tempoInicial) / CLOCKS_PER_SEC;

	printf("\n___________________________________");
	printf("\n            Resultados             ");
	printf("\n-----------------------------------");
	printf("\n# Tempo  de Execucao: %lf seg  ", tempoExecucao);
	printf("\n# Numero de Leituras: %d      ", analise.numLeitura);
	printf("\n# Numero de Escritas: %d      ", analise.numEscrita);
	printf("\n# Numero de Comparacoes: %lld   ", analise.numComparacoes);
	printf("\n __________________________________");
}

