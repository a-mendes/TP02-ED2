file = open("../data/PROVAO.TXT", "r")

# lê o arquivo e retorna uma lista 
def read_file(file):
    aluno = []
    for line in file:
        aluno.append(line.split()) # ['id', 'nota', 'estado', 'cidade', 'curso']
    return aluno

# ordena de forma crescente
def order_list(aluno):
    aluno.sort(key=lambda x: x[1]) # ordena de acordo com a segunda posição do vetor acima ['nota']
    return aluno

# gera o arquivo ordenado
def make_file(opcao):
    aluno = read_file(file)   # lê o arquivo
    if (opcao == 1):          # ordena de forma crescente
        aluno = order_list(aluno)
        file_out = open("../data/PROVAO_CRESCENTE.TXT", "w") # cria o arquivo de saída
        
        for i in aluno:
            file_out.write(" ".join(i) + "\n") # escreve no arquivo de saída
        file_out.close()

    elif (opcao == 2):        # ordena de forma decrescente
        aluno = order_list(aluno)
        aluno.reverse()

        file_out = open("../data/PROVAO_DECRESCENTE.TXT", "w") # cria o arquivo de saída
        
        for i in aluno:
            file_out.write(" ".join(i) + "\n") # escreve no arquivo de saída
        file_out.close()

make_file(1)
make_file(2)