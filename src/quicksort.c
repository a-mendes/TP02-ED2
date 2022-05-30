#include "quicksort.h"

/**
 * Conjunto de variáveis que armazena o desempenho do método 
 */
int numLeitura = 0;
int numEscrita = 0;
int numComparacoes = 0;
clock_t tempoExecucaoInicio;
clock_t tempoExecucaoFim;

/**
 * Funções usadas localmente
 */
void quicksortExterno(FILE *arqLi, FILE *arqEi,	FILE *arqLEs, int esq, int dir);
void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir, int *i, int *j);
void escolherArquivoPorSituacao(int situacao, char* nomeArquivo);
void exibirResultados(opcional);

void quicksort(int quantidade, int situacao, int opcional){
	
	/**
	 * Escolhendo o nome do arquivo a ser usado de acordo
	 * com a situação fornecida
	 * 		1 - arquivo ordenado ascendentemente pelas notas
	 * 		2 - arquivo ordenado descendentemente pelas notas
	 *		3 - arquivo desordenado aleatoriamente pelas notas 
	 */ 
	char nomeArquivo[30];
	escolherArquivoPorSituacao(situacao, nomeArquivo);

	/**
	 * Definição de ponteiros para o arquivo
	 */
	FILE *arqLi;
	FILE *arqEi;
	FILE *arqLEs;

	arqLi = fopen(nomeArquivo, "r+b");
	arqEi = fopen(nomeArquivo, "r+b");
	arqLEs = fopen(nomeArquivo, "r+b");

	if(arqLi == NULL || arqEi == NULL || arqLEs == NULL){
		printf("\nERRO: Falha ao abrir arquivo \"%s\"\n", nomeArquivo);
		return;
	}		

	/**
	 * Chamada para ordenação
	 */  
	tempoExecucaoInicio = clock();
	quicksortExterno(&arqLi, &arqEi, &arqLEs, 1, quantidade);
	tempoExecucaoFim = clock();

	/**
	 * Exibição de resultados de desempenho
	 */ 
	exibirResultados(opcional);
}

void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir){
	TipoArea area;
	int i, j;

	/**
	 * Se o indice da direita for menor ou igual ao indice da esquerda,
	 * significa que não há registros para serem ordenados ou as informações
	 * passadas por parâmetro estão incorretas.
	 */ 
	if(dir <= esq)
		return;

	//Define subarquivos
	particao(arqLi, arqEi, arqLEs, area, esq, dir, &i, &j);

	/**
	 * Verifica menor subarquivo
	 */ 
	if(i - esq < dir - j){
		quicksortExterno(arqLi, arqEi, arqLEs, esq, i);
		quicksortExterno(arqLi, arqEi, arqLEs, j, dir);
	}

	else {
		quicksortExterno(arqLi, arqEi, arqLEs, j, dir);
		quicksortExterno(arqLi, arqEi, arqLEs, esq, i);
	}
}

void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir, int *i, int *j){
	//implementar
}

void leSup(FILE **arqLEs, Alunos *ultimoLido, int *ls, short *ondeLer){
	//implementar
}

void leInf(FILE **arqLi, Alunos *ultimoLido, int *li, short *ondeLer){
	//implementar
}

void inserirArea(TipoArea *area, Alunos *ultimoLido, int *NRArea){
	//implementar
}

void escolherArquivoPorSituacao(int situacao, char* nomeArquivo){
	switch(situacao){
		case 1: nomeArquivo = "data/ProvaoAscendente.dat";
				break;
		case 2: nomeArquivo = "data/ProvaoDescendente.dat";
				break;
		case 3: nomeArquivo = "data/ProvaoAleatorio.dat";
				break; 
	}
}

void exibirResultados(opcional){
	/**
	 * Tempo de execução em segundos
	 */
	double tempoExecucao = ((double)t) / CLOCKS_PER_SEC;

	printf("\n______________________________");
	printf("\n          Resultados          ");
	printf("\n------------------------------");
	printf("\n• Tempo de Execucao: %lf seg  ", tempoExecucao);
	printf("\n• Numero de Leituras: %d      ", numLeitura);
	printf("\n• Numero de Escritas: %d      ", numEscrita);
	printf("\n• Numero de Comparacoes: %d   ", numComparacoes);
	printf("\n ____________________________ ");
}