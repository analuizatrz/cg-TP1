#include <stdio.h>
#include <stdlib.h>
#include "inimigo.h"

Inimigo *novoInimigo(float x,float y,float v){
    Inimigo * novo;
    // cria espaço para inimigo novo por alocação dinâmica
	novo = (Inimigo*)malloc(sizeof(Inimigo));
    //inicializa dados do inimigo
	novo->posicao.x = x;
	novo->posicao.y = y;
    novo->dimensao.altura = 40;
    novo->dimensao.largura = 40;
	novo->velocidade = v;
    novo->proximo = NULL;// aterra o ponteiro para o proximo
	return novo;
}
Inimigo *inserirInimigo(Inimigo *novo, Inimigo *lista){
    // se novo nao esta nulo
	if(novo != NULL){
        // se a lista esta vazia, novo é o primeiro
        if(lista == NULL){
            lista = novo;
        //senao, procura pelo ultimo
        }else{
            Inimigo *aux = lista;
            //percorre a lista ate aux ser o ultimo 
            while(aux->proximo != NULL){
                aux = aux->proximo;
            }
            //novo passa a ser o ultimo
            aux->proximo = novo;
        }
    }
	return lista;
}
Inimigo *excluirInimigo(Inimigo *inimigo, Inimigo *lista){
    //se nem lista nem inimigo forem nulos
	if(inimigo != NULL && lista != NULL){
        //se o inimigo a ser excluido e o primeiro da lista
        if(lista == inimigo){
            lista = inimigo->proximo;
            free(inimigo); // libera alocação de memoria
        //senao percorre a lista procurando pelo inimigo
        }else{
            Inimigo *aux = lista;
            while(aux->proximo != NULL){
                if(aux->proximo == inimigo){
                    aux->proximo = inimigo->proximo;//retira inimigo da lista
                    free(inimigo); // libera alocação de memoria
                }else{
                    aux = aux->proximo;//anda na lista
                }
            }
        }
    }
	return lista;
}
Inimigo *procurarInimigo(float x,float y, Inimigo *lista){
    Inimigo *aux = lista;
	while(aux != NULL){
		if(aux->posicao.x == x && aux->posicao.y == y)
            return aux;
        else
            aux = aux->proximo;
	}
    return NULL;
}
Inimigo *atualizaInimigos(Inimigo *lista,int *fugitivos){
    if(lista != NULL){
        Inimigo *aux = lista;
        //movimenta todos os inimigos da lista, somando a velocidade à posição
    	while(aux != NULL){
            aux->posicao.x += aux->velocidade; // acresce a velocidade na posição
    		aux = aux->proximo; // proximo
    	}
        /*cria um ponteiro auxiliar para guardar o endereço de memoria 
        do inimigo a ser removido*/
        Inimigo *removido;
        // se o primeiro da lista sair da tela, é removido
        if(lista->posicao.x > LIMITE_TELA_X){
            removido = lista; // guarda endereço
            lista = lista->proximo; // tira da lista
            free(removido); // libera memoria
            (*fugitivos)++;//atualiza numero de fugitivos
        }
        aux = lista;
        // percorre todos da lista
        while(aux != NULL && aux->proximo != NULL){
            // se há inimigo fora da tela
            if(aux->proximo->posicao.x > LIMITE_TELA_X){
                removido = aux->proximo; // guarda endereço
                aux->proximo = removido->proximo;//tira da lista
                free(removido); // libera memoria
                (*fugitivos)++;//atualiza numero de fugitivos
            }else{
                aux = aux->proximo; // proximo
            }
        }
    }
    return lista;
}
Inimigo *colisao(int x, int y, Inimigo *lista, int *atingidos){
    if(lista != NULL){ 
        Inimigo *removido;
        // confere se o tiro acertou o primeiro da lista
        if(dentroArea(x,y,lista)){
            removido = lista; // guarda endereço
            lista = lista->proximo; // tira da lista
            free(removido); // libera memoria
            (*atingidos)++;
        }
        Inimigo *aux = lista;
        //enquanto nao for o ultimo, verica se o tiro acertou aux
        while(aux != NULL && aux->proximo != NULL){
            //se o tiro acertou
            if(dentroArea(x,y,aux->proximo)){
                removido = aux->proximo;
                aux->proximo = removido->proximo;
                free(removido);
                (*atingidos)++;//atualiza numero de atingidos
            }else{
                aux = aux->proximo;
            }
        }
    }
    return lista;
}
int dentroArea(int x, int y, Inimigo *area){
    if(area->posicao.x < x && x < (area->posicao.x + 50)
    && area->posicao.y < y && y < (area->posicao.y + 50))
        return 1;//retorna true se x,y esta dentro da area limitada
    else
        return 0;
}
int limiteTela(Inimigo *inimigo){
    if(inimigo->posicao.x > LIMITE_TELA_X || inimigo->posicao.x < 0
    || inimigo->posicao.y > LIMITE_TELA_Y || inimigo->posicao.y < 0)
        return 1; // retorna true se o inimigo esta dentro da tela
    else
        return 0;
}