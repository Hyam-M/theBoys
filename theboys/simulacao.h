#ifndef SIMULACAO_H
#define SIMULACAO_H

#include "entidades.h"
#include "fprio.h"

/* structs usadas para passar os parâmetros em void *item */

/* struct que recebe um herói e uma base */
struct heroi_base {
    heroi *h ;
    base *b;
};

/* struct que recebe um herói e uma missao */
struct heroi_missao {
	heroi *h ;
	missao m ;
};

struct missaoItem {
	missao *m ;
	int tentativa;
};

/* Cria os eventos iniciais */
void simulacaoInicializa(struct fprio_t *simulacao, mundo *m);

void simulacaoLoop(struct fprio_t *simulacao, mundo *m);

void simulacaoDestroi(struct fprio_t *simulacao);

#endif