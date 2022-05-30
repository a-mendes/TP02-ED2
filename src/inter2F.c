#include "inter2F.h"

#include "../utils/arquivo.h"

void FF_imprime(Estrutura *v, int tam) {
    int cont = 0;
    printf("\n");
    for (int i = 0; i < tam; i++) {
        printf("%d fita: %d - %ld %d %s %s %s T/F: %d \n", cont++, v[i].posFita, v[i].aluno.inscricao, (int)v[i].aluno.nota, v[i].aluno.estado, v[i].aluno.cidade, v[i].aluno.curso, v[i].maior);
    }
    printf("\n");
}

void intercalacao2F(int quantidade, int situacao, int opcional) {
    FILE *prova = escolherArquivoPorSituacao(situacao);
    if (opcional) {
        printf("Arquivo a ser ordenado: \n");
        imprimirFita(prova, quantidade);
    }
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    nomeiaArquivo(nomes);

    FILE *arqvs[TOTALFITA];  // Apontador para as fitas
    criaArquivo(arqvs, nomes);

    int vetTam = 0;  // Estrutura que guarda os alunos em memória
    Estrutura alunosEmMemoria[FF_TAMFITAINT];
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    FF_geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);
    int tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, 0);
    int fitaSaida = FF_intercalacao(arqvs, alunosEmMemoria, 0, FF_POSFITAEXT, tamEntrada, 1);

    if (opcional) {
        printf("\n\n\nRESULTADO: FITA %d \n", fitaSaida);
        imprimirFita(arqvs[fitaSaida], quantidade);
    }

    fechaArq(arqvs);
    fclose(prova);
}

void esvaziarHeap(Estrutura alunosEmMemoria[FF_TAMFITAINT]) {
    Alunos alunoNulo = getAlunoVazio();
    for (int i = 0; i < 10; i++) {
        alunosEmMemoria[i].aluno = alunoNulo;
    }
}

void FF_geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int count = 0;
    int numfita = 0;
    Alunos alunoNulo = getAlunoVazio();

    while (*vetTam > 1 && count < quantidade) {
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
    *vetTam = numfita + 1;
    esvaziarHeap(alunosEmMemoria);

    for (int i = 0; i < TOTALFITA; i++) {
        rewind(arqvs[i]);
    }
}
/*
 * Salva na memória o primeiro valor de cada fita de entrada
 * retorna a quantidade de fitas com valores reais
 */
int FF_inicializarMemoriaIntercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaInicial) {
    Alunos aluno;
    int countFitasNaoVazias = 0;
    int firstIndex = getFirstCurrentIndex(fitaEntradaInicial);
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        if (fread(&aluno, sizeof(Alunos), 1, arqvs[firstIndex + i]) && aluno.nota != -1) {
            alunosEmMemoria[countFitasNaoVazias].aluno = aluno;
            alunosEmMemoria[countFitasNaoVazias].maior = false;
            alunosEmMemoria[countFitasNaoVazias].posFita = firstIndex + i;
            countFitasNaoVazias++;
        }
    }
    HEAP_CONSTROI(alunosEmMemoria, countFitasNaoVazias);
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

int FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual, int fitaSaidaAtual, int tamEntrada, int countNiveis) {
    Alunos vazio = getAlunoVazio();

    while (tamEntrada > 0) {
        fitaEntradaAtual = alunosEmMemoria[0].posFita;
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);

        Alunos aluno;
        bool novaFita = false;
        if (feof(arqvs[fitaEntradaAtual]) || !fread(&aluno, sizeof(Alunos), 1, arqvs[fitaEntradaAtual]) || aluno.nota == -1) {
            novaFita = remove_No(alunosEmMemoria, &tamEntrada);
        } else {
            alunosEmMemoria[0].posFita = fitaEntradaAtual;
            novaFita = substitui(alunosEmMemoria, &tamEntrada, aluno);
        }

        if (novaFita) {
            fwrite(&vazio, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);

            int newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, fitaEntradaAtual);
            int newFitaEntrada = alunosEmMemoria[0].posFita;
            if (newTamEntrada > 0) {
                if (fitaSaidaAtual + 1 <= getLastCurrentIndex(fitaSaidaAtual)) {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, fitaSaidaAtual + 1, newTamEntrada, countNiveis);
                } else {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, getFirstCurrentIndex(fitaSaidaAtual), newTamEntrada, countNiveis + 1);
                }
            } else {
                if (fitaSaidaAtual == getFirstCurrentIndex(fitaSaidaAtual) && countNiveis == 1) {
                    return fitaSaidaAtual;
                } else {
                    // limpa os arquivos de entrada ja percorridos
                    limparEntradas(arqvs, getFirstCurrentIndex(fitaEntradaAtual), getLastCurrentIndex(fitaEntradaAtual));
                    // preenche o heap para a proxima intercalacao
                    newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual));
                    return FF_intercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual), getFirstCurrentIndex(fitaEntradaAtual), newTamEntrada, 1);
                }
            }
        }
    }
    return fitaEntradaAtual;
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

void imprimirFita(FILE *arq, int tam) {
    Alunos teste;
    int cont = 0;
    rewind(arq);
    while (fread(&teste, sizeof(Alunos), 1, arq) && tam--) {
        if (teste.nota != -1) {
            printf("%d - ", cont++);
            printf("%ld ", teste.inscricao);
            printf("%.2f ", teste.nota);
            printf("%s ", teste.estado);
            printf("%s ", teste.cidade);
            printf("%s \n", teste.curso);
        }
    }
    rewind(arq);
}

void imprimirFitas(FILE *arqvs[TOTALFITA]) {
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
        printf("FITA %d \n", i + 1);
        getchar();
        printf("\n\n\n");
        rewind(arqvs[i]);
    }
}