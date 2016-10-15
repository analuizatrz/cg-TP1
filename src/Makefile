CC=gcc
OPCOES_COMPILADOR=-I. -I/usr/include/SOIL
TODOS_ARQUIVOS_PONTOH = inimigo.h
TODOS_ARQUIVOS_OBJ = main.o inimigo.o

%.o: %.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	$(CC) -o main $^ $(OPCOES_COMPILADOR) -lSOIL -lglut -lGL -lm

run: all
	./main

clean:
	rm *.o main
##(gnu) gcc -g -Wall -o main main.c inimigo.c -lglut -lGL -lm
