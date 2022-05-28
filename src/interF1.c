#include "interF1.h"

void HEAP_REFAZ(Estrutura *v, int esq, int dir) {
    int i = esq;
    int j = i * 2 + 1;
    Estrutura aux = v[i];

    while (j <= dir) {
        if (j < dir && ((v[j].aluno.nota > v[j + 1].aluno.nota || v[j].maior == true) && v[j + 1].maior != true)) {
            j = j + 1;  // j recebe o outro filho de i
        }
        if (j < dir && v[j].aluno.nota > v[j + 1].aluno.nota && v[j].maior == true && v[j + 1].maior == true) {
            j = j + 1; 
        }
        if (aux.aluno.nota <= v[j].aluno.nota && aux.maior != true) {
            break;
        }
        if (aux.aluno.nota >= v[j].aluno.nota &&  v[j].maior == true && aux.maior != true) {
            break;
        }

        v[i] = v[j];

        i = j;
        j = i * 2 + 1;
    }
    v[i] = aux;
}

void HEAP_CONSTROI(Estrutura *v, int tam) {
    int esq = (tam / 2) - 1;
    while (esq >= 0) {
        HEAP_REFAZ(v, esq, tam - 1);
        esq--;
    }
}

void Insere_No(Estrutura *v, Alunos elem, int *tam) {
    if (*tam > 19) {
        printf("\nUltrapassou tamanho limite\n");
        return;
    }
    if (*tam == 0) {
        v[*tam].aluno = elem;
        v[*tam].maior = false;
        *tam += 1;
    } else {
        v[*tam].aluno = elem;
        v[*tam].maior = false;
        *tam += 1;
        HEAP_CONSTROI(v, *tam);
    }
}

bool remove_No(Estrutura *v, int *tam) {
    if (*tam == 0) {
        return false;
    } else {
        v[0] = v[*tam - 1];
        *tam -= 1;
    }

    bool normal = true;
    for (int i = 0; i < *tam; i++) {
        if (v[i].maior == false) {
            normal = false;
            break;
        }
    }

    if(normal) {
        for (int i = 0; i < *tam; i++) {
            v[i].maior = false; 
        }
    }

    HEAP_CONSTROI(v, *tam);

    return normal;
}

bool substitui(Estrutura *v, int *tam, Alunos elem) {
    if (*tam == 0) {
        return false;
    } else {
        if (v[0].aluno.nota > elem.nota) {
            v[0].aluno = elem;                                                        
            v[0].maior = true;
        } else {
            v[0].aluno = elem;
            v[0].maior = false;
        }
    }

    bool normal = true;
    for (int i = 0; i < *tam; i++) {
        if (v[i].maior == false) {
            normal = false;
            break;
        }
    }

    if(normal) {
        for (int i = 0; i < *tam; i++) {
            v[i].maior = false; 
        }
    }
   
    HEAP_CONSTROI(v, *tam);

    return normal;
}























void imprime(Estrutura *v, int tam) {
    int cont = 0;
    printf("\n");
    for (int i = 0; i < tam; i++) {
        printf("%d - %ld %.1f %s %s %s T/F: %d \n", cont++, v[i].aluno.inscricao, v[i].aluno.nota, v[i].aluno.estado, v[i].aluno.cidade, v[i].aluno.curso, v[i].maior);
    }
    printf("\n");
}

// intercalacaoF1
int main() {
    // FILE* prova = fopen("../data/ProvaoAleatorio.dat", "rb");  // Arquivo de entrada

    FILE* prova = fopen("../data/AleatorioMenor.dat", "rb");  // teste

    char nomes[TOTALFITA][TOTALFITA] = {""};                                  // Vetor de nomes para criar as fitas
    FILE* arqvs[TOTALFITA];                                            // Apontador para as fitas
    Estrutura alunosEmMemoria[TAMFITAINT]; // voltat intam
    int vetTam = 0;                              // Estrutura que guarda os alunos em memória

    nomeiaArquivo(nomes);
    criaArquivo(arqvs, nomes);

    for (int i = 0; i < TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].maior = false;
        vetTam++;
    }

    HEAP_CONSTROI(alunosEmMemoria, vetTam);
    
    geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam);

    intercalacao(arqvs, alunosEmMemoria, &vetTam);

    imprimeFitaSaida(arqvs);

    fechaArq(arqvs);
    fclose(prova);

    return 0;
}


void nomeiaArquivo(char nomes[TOTALFITA][TOTALFITA]) {
    char buffer[3];
    char aux[TOTALFITA] = {"../data/fita"};

    for (int i = 1; i <= TOTALFITA; i++) {
        itoa(i, buffer, 10);
        strcat(aux, buffer);
        strcpy(nomes[i - 1], strcat(aux, ".dat"));
        aux[12] = '\0';
    }
}

void criaArquivo(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]) {
    for (int i = 0; i < 20; i++) {
        arqvs[i] = fopen(nomes[i], "wb+");
    }
}

Alunos readFile(FILE* prova) {
    Alunos aluno;

    fread(&aluno, sizeof(Alunos), 1, prova);

    return aluno;
}


void geraBlocos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], FILE *prova, int *vetTam) {
    int numfita = 0;
    Alunos alunoNulo;
    alunoNulo.nota = -1;
    
    while(*vetTam > 1) {
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[numfita]);
        
        if(feof(prova)) {
            if(remove_No(alunosEmMemoria, vetTam) || *vetTam == 1){
                fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
                numfita += 1;   
            }
        } 
        else if(substitui(alunosEmMemoria, vetTam, readFile(prova))) {
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[numfita]);
            numfita += 1;
        }

        if(numfita >= TAMFITAINT)
            numfita = 0;
 
    }

    *vetTam = numfita + 1;


    Alunos teste;
    int cont = 0;
    for(int i = 0; i < 19; i++){
        rewind(arqvs[i]);
        printf("FITA %d \n", i + 1);
        while(!feof(arqvs[i])) {
            fread(&teste, sizeof(Alunos), 1, arqvs[i]);

            printf("%d -",cont++);
            printf("%ld ", teste.inscricao);
            printf("%.2f ", teste.nota);
            printf("%s ", teste.estado);
            printf("%s ", teste.cidade);
            printf("%s \n", teste.curso);
        } 
        printf("\n\n\n\n\n\n\n\n\n\n");
    } 
}


void fechaArq(FILE* arqvs[TOTALFITA]) {
    for (int i = 0; i < TOTALFITA; i++) {
        fclose(arqvs[i]);
    }
}


void intercalacao(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    Alunos alunoNulo;
    alunoNulo.nota = -1;

    Alunos aluno;
 
    preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);

    while(*vetTam > 0) {
        fwrite(&alunosEmMemoria[0].aluno, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
        
        aluno = readFile(arqvs[alunosEmMemoria[0].posFita]);

        if(feof(arqvs[alunosEmMemoria[0].posFita]))
            aluno.nota = -1;

        if(aluno.nota == -1) {
            if(remove_No(alunosEmMemoria, vetTam)) {
               fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
            }
        }
        else if(substitui(alunosEmMemoria, vetTam, aluno)) {
            fwrite(&alunoNulo, sizeof(Alunos), 1, arqvs[POSFITAEXT]);
        }

        if(*vetTam == 0) {
            preencheVetorAlunos(arqvs, alunosEmMemoria, vetTam);
        }
    }
}


void preencheVetorAlunos(FILE *arqvs[TOTALFITA], Estrutura alunosEmMemoria[TAMFITAINT], int *vetTam) {
    if(*vetTam != 0) {
        for(int i = 0; i < *vetTam; i++) {
            rewind(arqvs[i]);
            fread(&alunosEmMemoria[i].aluno, sizeof(Alunos), 1, arqvs[i]);
            alunosEmMemoria[i].posFita = i;
            alunosEmMemoria[i].maior = false;
        }
    }
    else {
        for(int j = 0,  i = 0; i < TAMFITAINT; i++) {
            if(!feof(arqvs[i])){
                fread(&alunosEmMemoria[j].aluno, sizeof(Alunos), 1, arqvs[i]);
                alunosEmMemoria[j++].posFita = i;
                alunosEmMemoria[i].maior = false;
                *vetTam += 1;
            }
        }
    }

    HEAP_CONSTROI(alunosEmMemoria, *vetTam);
}

void imprimeFitaSaida(FILE *arqvs[TOTALFITA]) {
    Alunos aluno;
    int cont = 0;
    rewind(arqvs[POSFITAEXT]);
    printf("\n\nFITA DE SAIDA\n");   
        
    while(!feof(arqvs[POSFITAEXT])) {
        fread(&aluno, sizeof(Alunos), 1, arqvs[POSFITAEXT]);

        printf("%d -",cont++);
        printf("%ld ", aluno.inscricao);
        printf("%.2f ", aluno.nota);
        printf("%s ", aluno.estado);
        printf("%s ", aluno.cidade);
        printf("%s \n", aluno.curso);
    } 
    printf("\n\n");
}