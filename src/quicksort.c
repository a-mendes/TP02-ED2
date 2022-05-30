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
void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir);
void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea Area, int esq, int dir, int *i, int *j);

void leSup(FILE **arqLEs, Alunos *ultimoLido, int *ls, short *ondeLer);
void leInf(FILE **arqLi, Alunos *ultimoLido, int *li, short *ondeLer);

void inserirArea(TipoArea *area, Alunos *ultimoLido, int *NRArea);
void insereItem(Alunos *ultimoLido, TipoArea *area);

void escreveMax(FILE **arqLEs, Alunos R, int *es);
void escreveMin(FILE **arqEi, Alunos R, int *ei);

void retiraMax(TipoArea *area, Alunos *R, int *NRArea);
void retiraMin(TipoArea *area, Alunos *R, int *NRArea);
void retiraUltimo(TipoArea *area, Alunos *R);
void retiraPrimeiro(TipoArea *area, Alunos *R);

void criaAreaVazia(TipoArea *area);
int obterNumCelulasOcupadas(TipoArea *area);

void escolherArquivoPorSituacao(int situacao, char* nomeArquivo);
void exibirResultados(int opcional);

/**
 * Função chamada pela main
 */ 
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

	/**
	 * Cria área e define todas as células de área como NULL
	 */
	criaAreaVazia(&area);

	/**
	 * Define subarquivos
	 */ 
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

	/**
	 * Desaloca área
	 */ 
	free(&area);
}

void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea Area, int esq, int dir, int *i, int *j){
	int ls = dir, es = dir, li = esq, ei = esq, NRArea = 0, Linf = INT_MIN, Lsup = INT_MAX;
	short ondeLer = true; 
	Alunos ultimoLido, R;
	
	fseek(*arqLi, (li - 1) * sizeof(Alunos), SEEK_SET);
	fseek(*arqEi, (ei - 1) * sizeof(Alunos), SEEK_SET);
	
	*i = esq - 1; *j = dir + 1;
	
	/**
	 * A partição é feita até que Li e Ls se cruzem
	 */ 
	while(ls >= li){

		if(NRArea < TAM_MEMORIA_INTERNA - 1){	
			if (ondeLer)
				leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
			else 
				leInf(arqLi, &ultimoLido, &li, &ondeLer);
				
			inserirArea(&Area, &ultimoLido, &NRArea);
			continue;
		}
		
		/**
		 * Para garantir que os apontadores de escrita estejam atrás 
		 * dos apontadores de leitura, a ordem alternada de leitura é 
		 * interrompida se (Li = Ei) ou (Ls = Es).
		 */
		if(ls == es)
			leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
		
		else if(li == ei)
			leInf(arqLi, &ultimoLido, &li, &ondeLer);
		
		else if(ondeLer)
			leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
		
		else
			leInf(arqLi, &ultimoLido, &li, &ondeLer);
		
		if(ultimoLido.nota > Lsup){
			*j = es;
			escreveMax(arqLEs, ultimoLido, &es);
			continue;
		}

		if(ultimoLido.nota > Linf){
			*j = es;
			escreveMax(arqLEs, ultimoLido, &es);
			continue;
		}
		
		inserirArea(&Area, &ultimoLido, &NRArea);
		if(ei - esq < dir - es){
			retiraMin(&Area, &R, &NRArea);
			escreveMin(arqEi, R, &ei);
			
			Linf = R.nota;
		}

		else {
			retiraMax(&Area, &R, &NRArea);
			escreveMax(arqLEs, R, &es);
			
			Linf = R.nota;
		}
	}

	while (ei <= es){
		retiraMin(&Area, &R, &NRArea);
		escreveMin(arqEi, R, &ei);
	}
}


void leSup(FILE **arqLEs, Alunos *ultimoLido, int *ls, short *ondeLer){
	fseek(*arqLEs, (*ls - 1) * sizeof(Alunos), SEEK_SET);
	fread(ultimoLido, sizeof(Alunos), 1, *arqLEs);
	ls--; 
	*ondeLer = false;
}

void leInf(FILE **arqLi, Alunos *ultimoLido, int *li, short *ondeLer){
	fread(ultimoLido, sizeof(Alunos), 1, *arqLi);
	(*li)++; 
	*ondeLer = true;
}

void inserirArea(TipoArea *area, Alunos *ultimoLido, int *NRArea){
	insereItem(ultimoLido, area);
	*NRArea = obterNumCelOcupadas(area);
}

void escreveMax(FILE **arqLEs, Alunos R, int *es){
	fseek(*arqLEs, (*es - 1) * sizeof(Alunos), SEEK_SET);
	fwrite(&R, sizeof(Alunos), 1, *arqLEs);
	(*es)--;
}

void escreveMin(FILE **arqEi, Alunos R, int *ei){
	fwrite(&R, sizeof(Alunos), 1, *arqEi);
	(*ei)++;
}

void insereItem(Alunos *ultimoLido, TipoArea *area){
	//Implementar
}

void retiraMax(TipoArea *area, Alunos *R, int *NRArea){
	retiraUltimo(area, R);
	*NRArea = obterNumCelulasOcupadas(area);
}

void retiraMin(TipoArea *area, Alunos *R, int *NRArea){
	retiraPrimeiro(area, R);
	*NRArea = obterNumCelulasOcupadas(area);
}

void retiraUltimo(TipoArea *area, Alunos *R){
	//Implementar
}

void retiraPrimeiro(TipoArea *area, Alunos *R){
	//Implementar
}

void criaAreaVazia(TipoArea *area){
	area = malloc(sizeof(TipoArea) * TAM_MEMORIA_INTERNA);
	for (int i = 0; i < TAM_MEMORIA_INTERNA; i++){
		area[i].inscricao = -1;
	}
}

int obterNumCelulasOcupadas(TipoArea *area){
	int contador = 0;
	for (int i = 0; i < TAM_MEMORIA_INTERNA; i++)
		if(area[i].inscricao != -1)
			contador++;
	
	return contador;
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

void exibirResultados(int opcional){
	
	if(opcional){
		//Exibir registros
	}

	/**
	 * Tempo de execução em segundos
	 */
	double tempoExecucao = (((double) tempoExecucaoInicio) - ((double) tempoExecucaoFim)) / CLOCKS_PER_SEC;

	printf("\n______________________________");
	printf("\n          Resultados          ");
	printf("\n------------------------------");
	printf("\n• Tempo de Execucao: %lf seg  ", tempoExecucao);
	printf("\n• Numero de Leituras: %d      ", numLeitura);
	printf("\n• Numero de Escritas: %d      ", numEscrita);
	printf("\n• Numero de Comparacoes: %d   ", numComparacoes);
	printf("\n ____________________________ ");
}