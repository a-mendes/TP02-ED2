@echo off
del *.exe >nul 2>&1 && cd obj && del *.o >nul 2>&1 && cd ../data && del fita*.dat >nul 2>&1 && cd ..
gcc -c ./utils/heap.c -o ./obj/heap.o
gcc -c ./utils/arquivo.c -o ./obj/arquivo.o
gcc -c ./src/interF1.c -o ./obj/interF1.o
gcc -c ./src/inter2F.c -o ./obj/inter2F.o
gcc -c ./src/main.c -o ./obj/main.o
gcc ./obj/main.o ./obj/interF1.o ./obj/inter2F.o ./obj/arquivo.o ./obj/heap.o -o ordena.exe