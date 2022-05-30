#include "inter2F.h"

#include "../utils/arquivo.h"

void FF_imprime(Estrutura *v, int tam) {
    int cont = 0;
    printf("\n");
    for (int i = 0; i < tam; i++) {
        printf("%d - %ld %d %s %s %s T/F: %d \n", cont++, v[i].aluno.inscricao, (int)v[i].aluno.nota, v[i].aluno.estado, v[i].aluno.cidade, v[i].aluno.curso, v[i].maior);
    }
    printf("\n");
}

void intercalacao2F(int quantidade, int situacao, int opcional) {
    FILE *prova;
    char nomes[TOTALFITA][TOTALFITA] = {""};   // Vetor de nomes para criar as fitas
    FILE *arqvs[TOTALFITA];                    // Apontador para as fitas
    Estrutura alunosEmMemoria[FF_TAMFITAINT];  // voltat intam;
    int vetTam = 0;                            // Estrutura que guarda os alunos em memória

    switch (situacao) {
        case 1:
            // prova = fopen("./data/ProvaoAscendente.dat", "rb");
            break;
        case 2:
            // prova = fopen("./data/ProvaoDescendente.dat", "rb");
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

    for (int i = 0; i < FF_TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    FF_geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);

    int fitaSaida = FF_intercalacao(arqvs, alunosEmMemoria, 0);

    printf("FITA %d \n", fitaSaida);
    Alunos teste;
    int cont = 1;
    rewind(arqvs[fitaSaida]);
    while (fread(&teste, sizeof(Alunos), 1, arqvs[fitaSaida]) && teste.nota != -1) {
        printf("%d - ", cont++);
        printf("%ld ", teste.inscricao);
        printf("%.2f ", teste.nota);
        printf("%s ", teste.estado);
        printf("%s ", teste.cidade);
        printf("%s \n", teste.curso);
    }
    // Alunos teste;
    // int cont = 0;
    // for (int i = 0; i < 19; i++) {
    //     rewind(arqvs[i]);
    //     printf("FITA %d \n", i + 1);
    //     while (fread(&teste, sizeof(Alunos), 1, arqvs[i])) {
    //         if (teste.nota != -1) {
    //             printf("%d - ", cont++);
    //             printf("%ld ", teste.inscricao);
    //             printf("%.2f ", teste.nota);
    //             printf("%s ", teste.estado);
    //             printf("%s ", teste.cidade);
    //             printf("%s \n", teste.curso);
    //         }
    //     }
    //     printf("\n\n\n");
    //     rewind(arqvs[i]);
    // }

    fechaArq(arqvs);
    fclose(prova);
}

void FF_geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int count = 0;
    int numfita = 0;
    Alunos alunoNulo = getAlunoVazio();

    while (*vetTam > 1 && count < quantidade) {
        if (alunosEmMemoria[0].aluno.inscricao == 92) {
            printf("aaa");
            exit(1);
        }
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[numfita]);
        if (feof(prova)) {
            if (remove_No(alunosEmMemoria, vetTam)) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                numfita += 1;
            }
        } else if (substitui(alunosEmMemoria, vetTam, readFile(prova))) {
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
            numfita += 1;
        }

        if (*vetTam == 1)
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);

        if (numfita >= FF_TAMFITAINT)
            numfita = 0;
        count++;
    }
    printf("%d", count);
    *vetTam = numfita + 1;
    for (int i = 0; i < 10; i++) {
        alunosEmMemoria[i].aluno = alunoNulo;
    }
    Alunos teste;
    int cont = 0;
    for (int i = 0; i < 19; i++) {
        rewind(arqvs[i]);
        printf("FITA %d \n", i + 1);
        while (fread(&teste, sizeof(Alunos), 1, arqvs[i])) {
            if (teste.nota != -1) {
                printf("%d - ", cont++);
                printf("%ld ", teste.inscricao);
                printf("%.2f ", teste.nota);
                printf("%s ", teste.estado);
                printf("%s ", teste.cidade);
                printf("%s \n", teste.curso);
            }
        }
        printf("\n\n\n");
        rewind(arqvs[i]);
    }
}
/*
 * Salva na memória o primeiro valor de cada fita de entrada
 * retorna a quantidade de fitas nao vazias
 */
int FF_inicializarMemoriaIntercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaInicial) {
    int countFitasNaoVazias = 0;
    int firstIndex = getFirstCurrentIndex(fitaEntradaInicial);
    int lastIndex = getLastCurrentIndex(fitaEntradaInicial) + 1;
    int size = FF_TAMFITAINT;
    for (int i = 0; i < size; i++) {
        if (fread(&alunosEmMemoria[i].aluno, sizeof(Alunos), 1, arqvs[firstIndex + i])) {
            countFitasNaoVazias++;
            alunosEmMemoria[i].maior = false;
            alunosEmMemoria[i].posFita = i;
        } else {
            alunosEmMemoria[i].aluno = getAlunoVazio();
            alunosEmMemoria[i].maior = true;
            alunosEmMemoria[i].posFita = i;
        }
    }
    HEAP_CONSTROI(alunosEmMemoria, 10);
    return countFitasNaoVazias;
}

void limparEntradas(FILE *arqvs[TOTALFITA], int startIndex, int endIndex) {
    char nomes[TOTALFITA][TOTALFITA] = {""};
    nomeiaArquivo(nomes);
    for (int i = 0; i < TOTALFITA; i++) {
        if (i >= startIndex && i <= endIndex) {
            fclose(arqvs[i]);
            arqvs[i] = fopen(nomes[i], "wb+");
        } else {
            rewind(arqvs[i]);
        }
    }
}

int contadora = 1;

int FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual) {
    int fitaSaidaAtual = getFirstOppositeIndex(fitaEntradaAtual);
    limparEntradas(arqvs, getFirstCurrentIndex(fitaSaidaAtual), getLastCurrentIndex(fitaSaidaAtual));
    int tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, fitaEntradaAtual);

    while (tamEntrada > 1) {
        FF_imprime(alunosEmMemoria, 10);
        printf("%d - %d\n", contadora++, alunosEmMemoria[0].aluno.inscricao);
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);
        fitaEntradaAtual = alunosEmMemoria[0].posFita + getFirstCurrentIndex(fitaEntradaAtual);
        Alunos aluno = readFile(arqvs[fitaEntradaAtual]);
        if (alunosEmMemoria[0].aluno.nota == -1 && remove_No(alunosEmMemoria, &tamEntrada)) {
            Alunos vazio = getAlunoVazio();
            fwrite(&vazio, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);
            tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, getFirstCurrentIndex(fitaEntradaAtual));
            if (tamEntrada > 0) {
                // volta à primeira fita de saída
                fitaSaidaAtual = (fitaSaidaAtual + 1) > getLastCurrentIndex(fitaSaidaAtual) ? getFirstCurrentIndex(fitaSaidaAtual) : (fitaSaidaAtual + 1);
            } else {
                limparEntradas(arqvs, getFirstCurrentIndex(fitaEntradaAtual), getLastCurrentIndex(fitaEntradaAtual));
                FF_intercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual));
            }
        } else if (substitui(alunosEmMemoria, &tamEntrada, aluno)) {
            Alunos vazio = getAlunoVazio();
            fwrite(&vazio, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);
            tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, getFirstCurrentIndex(fitaEntradaAtual));
            if (tamEntrada > 0) {
                // volta à primeira fita de saída
                fitaSaidaAtual = (fitaSaidaAtual + 1) > getLastCurrentIndex(fitaSaidaAtual) ? getFirstCurrentIndex(fitaSaidaAtual) : (fitaSaidaAtual + 1);
            } else {
                limparEntradas(arqvs, getFirstCurrentIndex(fitaEntradaAtual), getLastCurrentIndex(fitaEntradaAtual));
                FF_intercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual));
            }
        }
    }

    printf("entrada %d saida %d tam %d", fitaEntradaAtual, fitaSaidaAtual, tamEntrada);

    return fitaSaidaAtual == getFirstCurrentIndex(fitaSaidaAtual) ? getFirstOppositeIndex(fitaEntradaAtual) : getFirstCurrentIndex(fitaEntradaAtual);
}

/*
 * input:   0-9|10-19
 * returns:  0 | 10
 */
int getFirstCurrentIndex(int currentFita) {
    return (currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT;
}
/*
 * input:   0-9|10-19
 * returns: 10 | 0
 */
int getFirstOppositeIndex(int currentFita) {
    return !(currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT;
}

/*
 * input:   0-9|10-19
 * returns:  9 | 19
 */
int getLastCurrentIndex(int currentFita) {
    return (currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT + FF_POSFITAEXT - 1;
}

/*
 * input:   0-9|10-19
 * returns:  19 | 9
 */
int getLastOppositeIndex(int currentFita) {
    return getLastCurrentIndex(getFirstOppositeIndex(currentFita));
}

/*
 * input:   0-9|10-19
 * returns:  1 | 0
 */
int isInFirstTape(int currentTape) {
    return currentTape <= 9;
}

Alunos getAlunoVazio() {
    Alunos aluno = (Alunos){-1};
    aluno.nota = (float)-1.0;
    return aluno;
}