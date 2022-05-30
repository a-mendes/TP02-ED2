#Windows Makefile

all: main.o inter2F.o interF1.o quicksort.o arquivo.o heap.o #generator.o
	@gcc ./obj/main.o ./obj/inter2F.o ./obj/interF1.o ./obj/quicksort.o ./obj/arquivo.o ./obj/heap.o -o ordena

main.o: ./src/main.c
	@gcc -c ./src/main.c -o ./obj/main.o

inter2F.o: ./src/inter2F.c
	@gcc -c ./src/inter2F.c -o ./obj/inter2F.o

interF1.o: ./src/interF1.c
	@gcc -c ./src/interF1.c -o ./obj/interF1.o

quicksort.o: ./src/quicksort.c
	@gcc -c ./src/quicksort.c -o ./obj/quicksort.o

arquivo.o: ./utils/arquivo.c
	@gcc -c ./utils/arquivo.c -o ./obj/arquivo.o

heap.o: ./utils/heap.c
	@gcc -c ./utils/heap.c -o ./obj/heap.o

# generator.o: ./utils/generator.c
# 	@gcc -c ./utils/generator.c -o ./obj/generator.o

run:
	@./ordena.exe

go:
	@make all
	@make run

clean:
	@del *.exe /q
	@cd ./obj && del *.o /q && \
	@cls