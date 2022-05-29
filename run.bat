cd obj && del *.o && cd ..
gcc -c ./utils/heap.c -o ./obj/heap.o
gcc -c ./src/interF1.c -o ./obj/interF1.o
gcc -c ./src/main.c -o ./obj/main.o
gcc ./obj/main.o ./obj/interF1.o ./obj/heap.o -o ordena.exe