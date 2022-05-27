def readFile():
    file = open("../data/PROVAO.TXT", "r")
    content = file.readlines()

    file.close()

    return content

def writeFile(content):
    file_out_sort = open("../data/PROVAO_CRESCENTE.TXT", "w")
    file_out_reverse = open("../data/PROVAO_DECRESCENTE.TXT", "w")

    list = []

    for line in content:
        list.append(line)
    
    list.sort(key=lambda x: float(x[1]))

    for i in range(len(list)):
        for j in range(len(list[i])):
            file_out_sort.write(list[i][j] + " ")
        file_out_sort.write("\n")

    list.reverse()

    for i in range(len(list)):
        for j in range(len(list[i])):
            file_out_reverse.write(list[i][j] + " ")
        file_out_reverse.write("\n")

    file_out_sort.close()
    file_out_reverse.close()

def main():
    content = readFile()
    writeFile(content)


main()
