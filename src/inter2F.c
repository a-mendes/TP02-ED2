#include "inter2F.h"

#include "../utils/arquivo.h"
#include "../utils/heap.h"

Analise analise;

void FF_imprime(Estrutura *v, int tam) {
    int cont = 0;
    printf("\n");
    for (int i = 0; i < tam; i++) {
        printf("%d fita: %d - %ld %d %s %s %s T/F: %d \n", cont++, v[i].posFita, v[i].aluno.inscricao, (int)v[i].aluno.nota, v[i].aluno.estado, v[i].aluno.cidade, v[i].aluno.curso, v[i].maior);
    }
    printf("\n");
}

void intercalacao2F(int quantidade, int situacao, int opcional) {
    analise.numComparacoes = 0;
    analise.numEscrita = 0;
    analise.numLeitura = 0;

    FILE *prova = escolherArquivoPorSituacao(situacao);
    if (opcional) {
        printf("Arquivo a ser ordenado: \n");
        imprimirFita(prova, quantidade);
    }
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    nomeiaArquivo(nomes);

    FILE *arqvs[TOTALFITA];  // Apontador para as fitas
    criaArquivo(arqvs, nomes);

    // início da fase de criação de blocos ordenados
    int vetTam = 0;
    Estrutura alunosEmMemoria[FF_TAMFITAINT];  // heap da memória
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    FF_geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);
    // fim da fase de criação de blocos ordenados

    // início da intercalação
    analise.tempoInicial = clock();
    int tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, 0);
    int fitaSaida = FF_intercalacao(arqvs, alunosEmMemoria, 0, FF_POSFITAEXT, tamEntrada, 1);
    analise.tempoFinal = clock();
    // fim da intercalação

    FF_exibirResultados(quantidade, opcional, arqvs, fitaSaida);

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
            if (remove_No(alunosEmMemoria, vetTam, &analise)) {
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                numfita += 1;
            }
        } else if (substitui(alunosEmMemoria, vetTam, readFile(prova), &analise)) {
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
 * Preenche o heap o primeiro valor de cada fita de entrada
 * retorna a quantidade de fitas com valores reais
 */
int FF_inicializarMemoriaIntercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaInicial) {
    Alunos aluno;
    int countFitasNaoVazias = 0;
    int firstIndex = getFirstCurrentIndex(fitaEntradaInicial);
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        if (fread(&aluno, sizeof(Alunos), 1, arqvs[firstIndex + i]) && aluno.nota != -1) {
            analise.numLeitura++;
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

/*
 * função recursiva para intercalação
 * retorna o índice da fita de saída ordenada com o resultado
 */
int FF_intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual, int fitaSaidaAtual, int tamEntrada, int countNiveis) {
    Alunos vazio = getAlunoVazio();

    while (tamEntrada > 0) {
        // guarda a fita de origem do item a ser inserido na saída
        fitaEntradaAtual = alunosEmMemoria[0].posFita;

        // escreve o item na saída
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);
        analise.numEscrita++;

        Alunos aluno;
        bool novaFita = false;
        analise.numLeitura++;
        // caso tenha acabado a fita de entrada, remove o ultimo item do heap
        if (feof(arqvs[fitaEntradaAtual]) || !fread(&aluno, sizeof(Alunos), 1, arqvs[fitaEntradaAtual]) || aluno.nota == -1) {
            novaFita = remove_No(alunosEmMemoria, &tamEntrada, &analise);
        } else {
            alunosEmMemoria[0].posFita = fitaEntradaAtual;
            // substitui o proximo item da fita de entrada no heap
            novaFita = substitui(alunosEmMemoria, &tamEntrada, aluno, &analise);
        }

        // caso não exista mais itens na fita, ou todos os itens do heap estão marcados
        if (novaFita) {
            // escreve o delimitador vazio na saida
            fwrite(&vazio, sizeof(Alunos), 1, arqvs[fitaSaidaAtual]);

            // verifica se há mais itens na fita de entrada
            int newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, fitaEntradaAtual);
            int newFitaEntrada = alunosEmMemoria[0].posFita;
            if (newTamEntrada > 0) {
                // caso haja entradas, escreve na próxima fita de saída ou volta para primeira, subindo um nível
                if (fitaSaidaAtual + 1 <= getLastCurrentIndex(fitaSaidaAtual)) {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, fitaSaidaAtual + 1, newTamEntrada, countNiveis);
                } else {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, getFirstCurrentIndex(fitaSaidaAtual), newTamEntrada, countNiveis + 1);
                }
            } else {
                // caso não haja mais entradas, verifica se apenas uma fita de saída foi escrita
                if (fitaSaidaAtual == getFirstCurrentIndex(fitaSaidaAtual) && countNiveis == 1) {
                    // caso sim, finaliza a intercalação retornando o índice da fita de saída
                    return fitaSaidaAtual;
                } else {
                    // limpa os arquivos de entrada
                    limparEntradas(arqvs, getFirstCurrentIndex(fitaEntradaAtual), getLastCurrentIndex(fitaEntradaAtual));
                    // preenche o heap para a próxima intercalação
                    newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual));

                    // faz a chamada recursiva trocando as entradas e saídas
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
    return currentTape < FF_POSFITAEXT;
}

Alunos getAlunoVazio() {
    Alunos aluno = (Alunos){-1};
    aluno.nota = (float)-1.0;
    return aluno;
}

void FF_exibirResultados(int quantidade, int opcional, FILE *arqvs[TOTALFITA], int fitaSaida) {
    if (opcional) {
        printf("\n\n\nRESULTADO: FITA %d \n", fitaSaida + 1);
        imprimirFita(arqvs[fitaSaida], quantidade);
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

void imprimirFita(FILE *arq, int tam) {
    Alunos teste;
    int cont = 1;
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
    int cont = 1;
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