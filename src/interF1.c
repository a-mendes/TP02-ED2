#include "interF1.h"

#include "../utils/arquivo.h"
#include "../utils/heap.h"


Analise analise; // Strutura para armazenar as informações de análise de eficiência do algoritmo.


int intercalacaoF1(int quantidade, int situacao, int opcional) {
    FILE *prova;                              // Arquivo de origem à ser ordenado
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
    // È feito a leitua inicial para preencher o vetor de alunos em memória principal.
    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        analise.numLeitura += 1;
        vetTam++;
    }

    // Constroi o Min heap(Ordenando com o pai sempre menor que os filhos) a partir dos alunos em memória e retorna o número de comparações feitas.
    analise.numComparacoes += HEAP_CONSTROI(alunosEmMemoria, vetTam); 

    // Começa a fase de geração dos blocos iniciais ordenados
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);

    // Começa a fase de intercalção dos blocos, ou seja, passar das fitas de entrada para a fita de saída.
    while (intercalacao(arqvs, alunosEmMemoria, &vetTam)) {
        // e então passar da fita de saida para as de entrada de forma ordenada.
        redistribuicao(arqvs, nomes, &vetTam);
    }
    analise.tempoFinal = clock();

    exibirResultados(opcional, arqvs);

    fechaArq(arqvs);
    fclose(prova);

    return 0;
}

// Geração dos blocos iniciais ordenados por seleção por substituição.
void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int numfita = 0;                    // Número da fitas de entrada utilizadas
    int cont = 17, segundoBloco = 0;    // Contador para saber a quantidade de elementos a ser lidos do arquivo de entrada(original)
    Alunos alunoNulo;                   // Aluno nulo tendo nota = -1, para marcar o fim de um bloco ou da fita.
    alunoNulo.nota = -1;

    while (*vetTam > 1) {
        cont++;
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[numfita]); // Escreve o primeiro aluno da memória principal para a fita de entrada.
        analise.numEscrita += 1;

        if (feof(prova) || cont >= quantidade) {  // Verifica se o arquivo de entrada está vazio  ou se a quantidade de elementos a seram lidos foi alcançada 
            if (remove_No(alunosEmMemoria, vetTam, &analise) || *vetTam == 1) { // e à partir disso remove cada primeiro elemento do vetor.
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                analise.numEscrita += 1;
                if (*vetTam != 1)
                    numfita += 1;
            }
        }
        else {
            analise.numLeitura += 1;
            if(substitui(alunosEmMemoria, vetTam, readFile(prova), &analise)) { // Substitui o primeiro elemento do vetor pelo próximo aluno 
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);          // lido do arquivo de entrada(original).
                analise.numEscrita += 1;
                numfita += 1;
            }
        }

        if (numfita >= TAMFITAINT) { // Verifica se a posição do vetor de fitas de entrada ultrapassou o limite(19).
            numfita = 0;
            segundoBloco = 1;
        }
    }

    *vetTam = segundoBloco; // Retorna se existe mais de um bloco em alguma fita.
}

// Fase de intercalção dos blocos, ou seja, passar das fitas de entrada para a fita de saída.
int intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    int blocosFitaSaida = 0;            // Número de blocos que estão na fita de saída, para saber se há somente 1 e a ordenação acabou.
    Alunos aluno;
    Alunos alunoNulo;
    alunoNulo.nota = -1;
    int segundoBloco = *vetTam;

    for (int i = 0; i < TOTALFITA; i++) { // Reiniciando a posição das fitas para o início
        rewind(arqvs[i]);
    }

    // Preenchendo o vetor de elementos com um elemento de cada fita de entrada utilizada.
    preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);

    while (*vetTam > 0) {
        // Escreve o primeiro elemento do vetor de elementos para a fita de saída.
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
        analise.numEscrita += 1;

        // Lê o próximo elemento da fita de entrada da qual saiu o elemento que foi escrito na fita de saída.
        if (!fread(&aluno, sizeof(Alunos), 1, arqvs[alunosEmMemoria[0].posFita]))
            aluno.nota = -1;

        analise.numLeitura += 1;
        // Caso aluno.nota = -1, significa que a bloco/fita de entrada está vazia, então ele é removido do vetor de elementos.
        if (aluno.nota == -1)
            remove_No(alunosEmMemoria, vetTam, &analise);
        else
            substitui(alunosEmMemoria, vetTam, aluno, &analise);

        if (*vetTam == 0) {
            blocosFitaSaida++;
            if (segundoBloco) // se houver mais de um bloco em alguma fita de entrada, então a ordenação NÂO acabou.
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
                analise.numEscrita += 1;
            preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);
        }
    }
    // retorna se há necessidade de fazer a redistribuição dos blocos ou se a ordenação acabou.
    return blocosFitaSaida > 1 ? 1 : 0;
}

// Preenche o vetor de Alunos com o primeiro elemento(ou primeiro elemento restante) de cada fita utilizada
void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    *vetTam = 0;

    for (int j = 0, i = 0; i < TAMFITAINT; i++) {
        analise.numLeitura += 1;
        if (fread(&alunosEmMemoria[j].aluno, sizeof(Alunos), 1, arqvs[i])) { // Verifica se há elemento em cada fita de entrada e  o escreve no vetor de alunos.
            alunosEmMemoria[j].posFita = i;                                  // Guarda a posição da fita de entrada em cada elemento.
            alunosEmMemoria[j++].maior = false;
            *vetTam += 1;                                                    // Incrementa o tamanho do vetor de alunos.    
        }
    }
    analise.numComparacoes += HEAP_CONSTROI(alunosEmMemoria, *vetTam);
}

// Transfere os elementos da fita de saída para as fitas de entrada de forma ordenada.
void redistribuicao(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA], int *vetTam) {
    Alunos aux;
    int numFita = 0;          // Número de fitas de entrada utilizadas.
    *vetTam = 0;

    // Apaga toda informação das fitas de entrada.
    for (int i = 0; i < TAMFITAINT; i++) {
        arqvs[i] = freopen(nomes[i], "wb", arqvs[i]);
        arqvs[i] = freopen(nomes[i], "wb+", arqvs[i]);
    }
    rewind(arqvs[POSFITAEXT]); // Reinicia a posição da fita de saída.

    // Lê um elemento da fita de saída 
    while (fread(&aux, sizeof(Alunos), 1, arqvs[POSFITAEXT])) {
        analise.numLeitura += 1;

        if (aux.nota != -1) { // e caso o bloco/fita não tenha acabado
            fwrite(&aux, sizeof(Alunos), 1, arqvs[numFita]); // escreve o elemento na fita de entrada atual(numFIta).
            analise.numEscrita += 1;
        }
        else {
            fwrite(&aux, sizeof(Alunos), 1, arqvs[numFita++]); // Caso o bloco/fita tenha acabado, escreve o elemento nota = -1 na 
            analise.numEscrita += 1;                           // fita de entrada atual(numFIta) e incrementa numFIta.
            if (numFita == TAMFITAINT) {
                numFita = 0;
                *vetTam = 1;
            }
        }
    }
    // Apaga toda informação da fita de saída.
    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb", arqvs[POSFITAEXT]);
    arqvs[POSFITAEXT] = freopen(nomes[POSFITAEXT], "wb+", arqvs[POSFITAEXT]);
}

// Funçãos que imprime todos elementos da fita de saída.
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

// Funçaõ que imprime o resultado da analise de execução do programa, e caso solicitado imprime os elementos da fita de saída.
void exibirResultados(int opcional, FILE *arqvs[TOTALFITA]) {
    if (opcional) {
        imprimeFitaSaida(arqvs);
    }

    //Tempo de execução em segundos
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

