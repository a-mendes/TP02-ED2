#include "quicksort.h"

/**
 * Conjunto de variáveis que armazena o desempenho do método 
 */
int numLeituraQuick = 0;
int numEscritaQuick = 0;
int numComparacoesQuick = 0;
clock_t tempoExecucaoInicioQuick;
clock_t tempoExecucaoFimQuick;

/**
 * Funções usadas localmente
 */
void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir);
void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea *area, int esq, int dir, int *i, int *j);

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

TipoArea* criaAreaVazia(TipoArea *area);
int obterNumCelulasOcupadas(TipoArea *area);
void ordenaArea(TipoArea *area);
void exibeArea(TipoArea *area);


void copiarAluno(Alunos *destino, Alunos origem);

void escolherArquivoPorSituacaoQuick(int situacao, char* nomeArquivo);
void exibirResultadosQuick(int opcional, char *nomeArquivo, int quantidade);

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
	escolherArquivoPorSituacaoQuick(situacao, nomeArquivo);

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
	tempoExecucaoInicioQuick = clock();
	quicksortExterno(&arqLi, &arqEi, &arqLEs, 1, quantidade);
	tempoExecucaoFimQuick = clock();

	fclose(arqLi);
	fclose(arqEi);
	fclose(arqLEs);

	/**
	 * Exibição de resultados de desempenho
	 */ 
	exibirResultadosQuick(opcional, nomeArquivo, quantidade);
}

void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir){
	TipoArea *area;
	int i, j;

	/**
	 * Se o indice da direita for menor ou igual ao indice da esquerda,
	 * significa que não há registros para serem ordenados ou as informações
	 * passadas por parâmetro estão incorretas.
	 */ 
	if(dir <= esq)
		return;

	/**
	 * Malloca área e define todas as células com valores inválidos
	 */
	area = criaAreaVazia(area);

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
	free(area);
}

void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea *area, int esq, int dir, int *i, int *j){
	int ls = dir, es = dir, li = esq, ei = esq, NRArea = 0, Linf = INT_MIN, Lsup = INT_MAX;
	short ondeLer = true; 
	Alunos ultimoLido, R;
	
	fseek(*arqLi, (li - 1) * sizeof(Alunos), SEEK_SET);
	fseek(*arqEi, (ei - 1) * sizeof(Alunos), SEEK_SET);
	
	*i = esq - 1; 
	*j = dir + 1;
	
	/**
	 * A partição é feita até que Li e Ls se cruzem
	 */ 
	while(ls >= li){
		if(NRArea < TAM_MEMORIA_INTERNA - 1){	
			if (ondeLer)
				leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
			else 
				leInf(arqLi, &ultimoLido, &li, &ondeLer);
				
			inserirArea(area, &ultimoLido, &NRArea);
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
		
		numComparacoesQuick++;
		if(ultimoLido.nota > Lsup){
			*j = es;
			escreveMax(arqLEs, ultimoLido, &es);
			continue;
		}

		numComparacoesQuick++;
		if(ultimoLido.nota < Linf){
			*i = ei;
			escreveMin(arqEi, ultimoLido, &ei);
			continue;
		}
		
		inserirArea(area, &ultimoLido, &NRArea);
		
		if(ei - esq < dir - es){
			retiraMin(area, &R, &NRArea);
			escreveMin(arqEi, R, &ei);
			
			Linf = R.nota;
		}

		else {
			retiraMax(area, &R, &NRArea);
			escreveMax(arqLEs, R, &es);
			
			Lsup = R.nota;
		}
	}

	while (ei <= es){
		retiraMin(area, &R, &NRArea);
		escreveMin(arqEi, R, &ei);
	}
}


void leSup(FILE **arqLEs, Alunos *ultimoLido, int *ls, short *ondeLer){
	fseek(*arqLEs, (*ls - 1) * sizeof(Alunos), SEEK_SET);
	fread(ultimoLido, sizeof(Alunos), 1, *arqLEs);

	//printf("\nLs: %d", *ls);

	(*ls)--; 

	*ondeLer = false;

	numLeituraQuick++;
}

void leInf(FILE **arqLi, Alunos *ultimoLido, int *li, short *ondeLer){
	fread(ultimoLido, sizeof(Alunos), 1, *arqLi);


	//printf("\nLi: %d", *li);
	(*li)++; 

	*ondeLer = true;

	numLeituraQuick++;
}

void inserirArea(TipoArea *area, Alunos *ultimoLido, int *NRArea){
	insereItem(ultimoLido, area);
	*NRArea = obterNumCelulasOcupadas(area);
}

void escreveMax(FILE **arqLEs, Alunos R, int *es){
	fseek(*arqLEs, (*es - 1) * sizeof(Alunos), SEEK_SET);
	fwrite(&R, sizeof(Alunos), 1, *arqLEs);
	(*es)--;

	numEscritaQuick++;
}

void escreveMin(FILE **arqEi, Alunos R, int *ei){
	fwrite(&R, sizeof(Alunos), 1, *arqEi);
	(*ei)++;

	numEscritaQuick++;
}

void insereItem(Alunos *ultimoLido, TipoArea *area){
	/**
	 * Variável i representa o indice da primeira celula desocupada da área
	 */ 
	int i = obterNumCelulasOcupadas(area);
	
	/**
	 * Copia as informações do ultimo lido para a primeira celula desocupada da área
	 */
	copiarAluno(area[i], *ultimoLido);

	ordenaArea(area);
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
	/**
	 * Variável i representa o indice da última célula da área
	 */
	int i = obterNumCelulasOcupadas(area) - 1;
	copiarAluno(R, *area[i]);

	/**
	 * Valores inválidos que representam célula vazia
	 */ 
	area[i]->inscricao = -1;
	area[i]->nota = FLT_MAX;
}

void retiraPrimeiro(TipoArea *area, Alunos *R){
	/**
	 * Variável i representa o primeiro indice da área
	 */
	int i = 0;
	copiarAluno(R, *area[i]);

	/**
	 * Valores inválidos que representam célula vazia
	 */ 
	area[i]->inscricao = -1;
	area[i]->nota = FLT_MAX;

	/**
	 * Reordenando
	 */ 
	ordenaArea(area);
}

TipoArea* criaAreaVazia(TipoArea *area){
	area = malloc(sizeof(TipoArea) * TAM_MEMORIA_INTERNA);
	for (int i = 0; i < TAM_MEMORIA_INTERNA; i++){
		Alunos *aluno = malloc(sizeof(Alunos));
		aluno->inscricao = -1;
		aluno->nota = FLT_MAX;

		area[i] = aluno;
	}

	return area;
}

void exibeArea(TipoArea *area){
	printf("\n_________________________________");
	printf("\n               Area              ");
	printf("\n---------------------------------");
	
	for(int i = 0; i < TAM_MEMORIA_INTERNA; i++){
		printf("\n%d) %ld \t%.2f \t%s \t%s \t%s", 
			i, area[i]->inscricao, area[i]->nota, area[i]->estado, area[i]->cidade, area[i]->curso);
	}

	printf("\n______________________________________");
}

void ordenaArea(TipoArea *area){
	/**
	 * O método Shell Sort foi escohido para a ordenação da área
	 * pois sua implementação independe de recursão
	 */ 

	int n = TAM_MEMORIA_INTERNA;
	int h;
	for(h = 1; h < n; h = 3 * h + 1);//h inicial
	
	do {
		h = (h - 1) / 3;

		for(int i = h; i < n; i++){
			Alunos *aux = area[i];
			int j = i;
			
			while(area[j-h]->nota > aux->nota){
				area[j] = area[j-h];
				j = j - h;
				
				if(j < h)
					break;
			}
			area[j] = aux;
		}
	} while (h != 1);

	//exibeArea(area);
}

int obterNumCelulasOcupadas(TipoArea *area){
	int contador = 0;
	for (int i = 0; i < TAM_MEMORIA_INTERNA; i++){
		if(area[i]->inscricao != -1)
			contador++;
	}

	return contador;
}

void copiarAluno(Alunos *destino, Alunos origem){
	destino->inscricao = origem.inscricao;
	destino->nota = origem.nota;
	strcpy(destino->estado, origem.estado);
	strcpy(destino->cidade, origem.cidade);
	strcpy(destino->curso, origem.curso);
}

void escolherArquivoPorSituacaoQuick(int situacao, char* nomeArquivo){
	switch(situacao){
		case 1: strcpy(nomeArquivo, "data/ProvaoAscendente.dat");
				break;
		case 2: strcpy(nomeArquivo, "data/ProvaoDescendente.dat");
				break;
		case 3: strcpy(nomeArquivo, "data/ProvaoAleatorio.dat");
				break; 
	}
}

void exibirResultadosQuick(int opcional, char *nomeArquivo, int quantidade){
	
	if(opcional){

		FILE *arq;
		arq = fopen(nomeArquivo, "r+b");

		printf("\n______________________________________");
		printf("\n               Registros              ");
		printf("\n--------------------------------------");
		
		for(int i = 0; i < quantidade; i++){
			Alunos atual;
			fread(&atual, sizeof(Alunos), 1, arq);

			printf("\n%d) %ld \t%.2f \t%s \t%s \t\t\t%s", 
				i+1, atual.inscricao, atual.nota, atual.estado, atual.cidade, atual.curso);
		}

		fclose(arq);

		printf("\n______________________________________");

	}

	/**
	 * Tempo de execução em segundos
	 */
	double tempoExecucao = (((double) tempoExecucaoFimQuick) - ((double) tempoExecucaoInicioQuick)) / CLOCKS_PER_SEC;

	printf("\n______________________________");
	printf("\n          Resultados          ");
	printf("\n------------------------------");
	printf("\n- Tempo de Execucao: %lf seg  ", tempoExecucao);
	printf("\n- Numero de Leituras: %d      ", numLeituraQuick);
	printf("\n- Numero de Escritas: %d      ", numEscritaQuick);
	printf("\n- Numero de Comparacoes: %d   ", numComparacoesQuick);
	printf("\n ____________________________ ");
}