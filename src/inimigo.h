#ifndef _INIMIGO_H
#define _INIMIGO_H

#define LIMITE_TELA_X 500// limite da tela em x
#define LIMITE_TELA_Y 500// limite da tela em y

struct ponto{
    float x;
    float y;
};

struct tamanho{
    float largura;
    float altura;
};

struct inimigo{
	float velocidade;
    struct ponto posicao;
    struct tamanho dimensao;
    struct inimigo *proximo;
};

typedef struct inimigo Inimigo;
/*
** Cria novo inimigo por alocação dinâmica, e o retorna
*/
Inimigo *novoInimigo(float x,float y,float v);
/*
** Cria insere um inimigo novo em uma lista
*/
Inimigo *inserirInimigo(Inimigo *novo, Inimigo *lista);
/*
** Cria exclui inimigo da lista
*/
Inimigo *excluirInimigo(Inimigo *inimigo, Inimigo *lista);
/*
** Procura inimigo na lista a partir das coordenadas x e y (posição)
*/
Inimigo *procurarInimigo(float x,float y, Inimigo *lista);
/*
** Atualiza a posição dos inimigos (soma com a respectiva velocidade)
** e elimina os que sairam dos limites da tela
*/
Inimigo *atualizaInimigos(Inimigo *lista, int *fugitivos);
/*
** Verifica se as coordenadas x e y estão contidas dentro da area de 
** algum inimigo da lista, eliminando-os
*/
Inimigo *colisao(int x, int y, Inimigo *lista, int *atingidos);
/*
** Verifica se as coordenadas x e y estão contidas na area de um inimigo
*/
int dentroArea(int x, int y, Inimigo *area);
/*
** Verifica se o inimigo esta fora dos limites da tela
*/
int limiteTela(Inimigo *inimigo);

#endif
