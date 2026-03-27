#ifndef EVENTOS_H
#define EVENTOS_H

#include "entidades.h"
#include "fprio.h"

typedef struct {
    int id;
    int dist;
} baseDist;

void EventoChega(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoEspera(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoDesiste(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoAvisa(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoEntra(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoSai(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoViaja(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoMorre(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoMissao(struct fprio_t *simulacao, int instante, void *item, mundo *m);

void EventoFim(int instante, mundo *m);

#endif