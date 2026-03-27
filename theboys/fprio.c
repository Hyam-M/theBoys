#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
/*
// descreve um nodo da fila de prioridades
// NAO altere estas estruturas
struct fpnodo_t
{
  void *item ;          // item associado ao nodo
  int   tipo ;          // tipo do item
  int   prio ;          // prioridade do item
  struct fpnodo_t *prox;    // próximo nodo
};

// descreve uma fila de prioridades
struct fprio_t
{
  struct fpnodo_t *prim ;   // primeiro nodo da fila
  struct fpnodo_t *fim;		// último nodo da fila 
  int num ;         // número de itens na fila
} ;
*/
struct fprio_t *fprio_cria () {
	struct fprio_t *fila = malloc(sizeof(struct fprio_t));
	if (fila) {
		fila->prim = NULL;
		fila->fim = NULL;
		fila->num = 0;
		return fila;
	}
	return NULL;
}

struct fprio_t *fprio_destroi (struct fprio_t *f) {
	if (f == NULL) {
		return NULL;
	}

	int tipo, prio;

	while(f->prim != NULL) {
		void *item = fprio_retira(f, &tipo, &prio);
		free(item);
	}
	free(f);
	return NULL;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
	struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t));

    	if (novo == NULL || f == NULL || (item == NULL)) {
		free(novo);
		return -1;
	}
	/*constroi o novo nodo*/
	novo->prox = NULL;
	novo->item = item;
	novo->tipo = tipo;
	novo->prio = prio;

	/*caso a lista esteja vazia*/
	if(f->prim == NULL) {
		f->prim = novo;
		f->fim = novo;
		f->num++;
		return 1;
	}
	if(f->prim->item == novo->item) {
		free(novo);
		return -1;
	}
	/*adiciona no primeiro lugar*/
	if(f->prim->prio > novo->prio) {
		novo->prox = f->prim;
		f->prim = novo;
		f->num++;
		return 1;
	}
	
	struct fpnodo_t *aux = f->prim;
	while(aux->prox != NULL && aux->prox->prio <= novo->prio) {
		if (aux->prox->item == novo->item) {
			free(novo);
			return -1;
		}
		aux = aux->prox;
	}
	/*Se o novo for inserido no fim*/
	if (aux->prox == NULL) {
		f->fim = novo;
	}

	novo->prox = aux->prox;
	aux->prox = novo;
	f->num++;
	return 1;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
	if (f == NULL || f->prim == NULL || tipo == NULL || prio == NULL) {
		return NULL;
	}
	//aux recebe o primeiro nodo.
	struct fpnodo_t *aux = f->prim;
	//item recebe o item do primeiro nodo.
	void *item = f->prim->item;
	//variáveis do elemento a ser removido.
	*tipo = f->prim->tipo;
	*prio = f->prim->prio;
	//O prim vira o segundo.
	f->prim = f->prim->prox;
	//o primeiro nodo é liberado.
	free(aux);
	//tamanho da fila é decrementado;
	f->num--;
	//retorna um ponteiro para o item.
	return item;
}

int fprio_tamanho (struct fprio_t *f) {
	if (f != NULL) {
		return f->num;
	}
	return -1;
}

void fprio_imprime (struct fprio_t *f) {
	if (f != NULL && f->num > 0) {
		struct fpnodo_t *aux = f->prim;
		while (aux->prox != NULL) {
			printf("(%d %d) ", aux->tipo, aux->prio);
			aux = aux->prox;
		}
		printf("(%d %d)\n", aux->tipo, aux->prio);
	}
}

