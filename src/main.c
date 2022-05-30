#include "inter2F.h"
#include "interF1.h"
#include "quicksort.h"

// ordena <método> <quantidade> <situação> [-P]

int main(int argc, char const *argv[]) {
    printf("\n===== TP1 - Estrutura de Dados II =====\n\n");

    int metodo = -1;      // metodo de pesquisa a ser executado (1 - 2F fitas, 2 - F+1 fitas, 3 - Quicksort)
    int quantidade = -1;  // quantidade de registros do arquivo (100, 200, 2k, 20k, 200k, 471.705)
    int situacao = 1;     // situação de ordem do arquivo (1 - Crescente, 2 - Decrescente, 3 - Desordenado)
    int opcional = 0;     // parâmetro opcional (1 - imprime os registros, 0 - não imprime os registros)

    if (argv[1] != NULL) {
        metodo = atoi(argv[1]);

        if (argv[2] != NULL) {
            quantidade = atoi(argv[2]);

            if (argv[3] != NULL && situacao > 0 && situacao < 4) {
                situacao = atoi(argv[3]);

                if (argv[4] != NULL) {
                    opcional = atoi(argv[4]);
                }
            }
        }
    }

    /** 
     * Implementar aqui chamadas para geração de arquivos ordenados 
     * ascendentemente e descendentemente
     */
    
    //Menu de acesso às pesquisas
    switch (metodo) {
        case 1:
            //intercalacao2F
            break;

        case 2:
            intercalacaoF1(quantidade, situacao, opcional);
            break;

        case 3:
            quicksort(quantidade, situacao, opcional);
            break;

        default:  // opção inválida
            printf("Opcao invalida!\n");
            break;
    }

    return 0;
}