#include "quicksort.h"

void escolherArquivoPorSituacao(int situacao, char* nomeArquivo);

void quicksort(int quantidade, int situacao, int opcional)
{
	/**
	 * Escolhendo o nome do arquivo a ser usado de acordo
	 * com a situação fornecida
	 * 		1 - arquivo ordenado ascendentemente pelas notas
	 * 		2 - arquivo ordenado descendentemente pelas notas
	 *		3 - arquivo desordenado aleatoriamente pelas notas 
	 */ 

	char nomeArquivo[30];
	escolherArquivoPorSituacao(situacao, nomeArquivo);

	
}

void escolherArquivoPorSituacao(int situacao, char* nomeArquivo){
	switch(situacao)
	{
		case 1: nomeArquivo = "data/ProvaoAscendente.dat";
				break;
		case 2: nomeArquivo = "data/ProvaoDescendente.dat";
				break;
		case 3: nomeArquivo = "data/ProvaoAleatorio.dat";
				break;
	}
}