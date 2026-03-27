#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "fila.h"
#include "conjunto.h"

typedef struct local {
	int x;
	int y;
} local;

typedef struct heroi {
	int id ; //a identificação do herói
	struct cjto_t *habilidades ; //conjunto de int aleatório que representa as hab. do herói 
	int paciencia ; 
	int velocidade ; 
	int xp ; // experiência do herói
	int base ; // id da base em que se encontra
    int status ; //1 se vivo, 0 se morto.
} heroi;

typedef struct base {
	int id ; //id da base
	int lotacao; // número máximo de heróis.
	struct cjto_t *presentes ; // conjunto dos heróis presentes.
	struct fila_t *espera ; // Fila de espera para entrar na base.
	local local; //local da base.
} base ;

typedef struct missao {
	int id ;
	struct cjto_t *habilidades ; // Habilidades necessárias para concluir a missão.
	local local ; //local da missão.
} missao;

typedef struct mundo {
	int Nherois ;
	heroi *herois ; //ponteiro para heroi para fazer um vetor dinâmico 
	int Nbases ;
	base *bases ; //ponteiro para bases(será alocado um vetor de base).
	int Nmissoes ;
	missao *missoes ; //ponteiro para missao(será alocado um vetor de missao).
	int Nhabilidades ;
	int NcompostosV ; // quantidade de composto V.
	local mapa ; // Área do mapa.
	int relogio ; //tempo atual do mundo.

	/* ESTATISTICAS */
	int eventos_tratados;
	int missoes_cumpridas;
	int tent_min;
	int tent_max;
	int tent_total;
	int num_mortos;
	int *fila_max;
	int *base_missao;
} mundo;

mundo *inicializa_mundo ();

void destroi_mundo(mundo *mundo);

#endif