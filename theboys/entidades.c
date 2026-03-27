#include <stdlib.h>
#include "entidades.h"
#include "defines.h"

void inicializa_herois (heroi *herois) {
	for (int i = 0; i < N_HEROIS; i++) {
		herois[i].id = i;
		herois[i].xp = 0;
		herois[i].paciencia = rand() % 101;
		herois[i].velocidade = 50 + rand() % (5000 - 50 + 1); //dar uma revisada
		herois[i].habilidades = cjto_aleat(1 + rand() % 3 , N_HABILIDADES);
		herois[i].status = 1; 
	}
}

void inicializa_bases(base *base) {
	for (int i = 0; i < N_BASES; i++) {
		base[i].id = i;
		base[i].local.x = rand() % N_TAMANHO_MUNDO;
	    base[i].local.y = rand() % N_TAMANHO_MUNDO;
		base[i].lotacao = 3 + rand() % (10 -3 + 1); 
		base[i].presentes = cjto_cria(N_HEROIS); //conjunto vazio com capacidade de armazenamento de [0, N_HEROIS]
		base[i].espera = fila_cria(); //fila vazia;	
	}
}

void inicializa_missoes (missao *missao) {
	for(int i = 0; i < N_MISSOES; i++) {
		missao[i].id = i;
		missao[i].local.x = rand() % N_TAMANHO_MUNDO;
		missao[i].local.y = rand() % N_TAMANHO_MUNDO;
		missao[i].habilidades = cjto_aleat(6 + rand() % 5, N_HABILIDADES); //dar uma olhada.
	}	
}

mundo *inicializa_mundo () {
	mundo *mundo = malloc(sizeof(struct mundo));
	
	if (!mundo) {
		return NULL;
	}
	/****************INICIALIZA HEROIS********************/
	mundo->Nherois = N_HEROIS;
	mundo->herois = malloc(mundo->Nherois * sizeof(heroi));
	inicializa_herois(mundo->herois);
	
	/****************INICIALIZA BASES********************/
	mundo->Nbases = N_BASES;
	mundo->bases = malloc(mundo->Nbases * (sizeof(base)));
	inicializa_bases(mundo->bases);

	/****************INICIALIZA MISSOES********************/
	mundo->Nmissoes = N_MISSOES;
    mundo->missoes = malloc(mundo->Nmissoes * (sizeof(missao)));
	inicializa_missoes(mundo->missoes);
	
	/****************INICIALIZA RESTO********************/
	mundo->Nhabilidades = N_HABILIDADES;
	mundo->NcompostosV = N_COMPOSTOS_V;
	mundo->mapa.x = N_TAMANHO_MUNDO;
	mundo->mapa.y = N_TAMANHO_MUNDO;
	mundo->relogio = T_INICIO;

	/****************INICIALIZA ESTATISTICAS********************/
	mundo->eventos_tratados = 0;
	mundo->missoes_cumpridas = 0;
	mundo->num_mortos = 0;
	mundo->tent_min = 1000000;
	mundo->tent_max = 0;
	mundo->tent_total = 0;
	mundo->fila_max = malloc(N_BASES*sizeof(int));
	mundo->base_missao = malloc(N_BASES*sizeof(int));
	for (int i = 0; i < N_BASES; i++) {
		mundo->fila_max[i] = 0;
		mundo->base_missao[i] = 0;
	}

	return mundo;
}

void destroi_herois(heroi *herois) {
	for(int i = 0; i < N_HEROIS; i++) {
		cjto_destroi(herois[i].habilidades);
	}
	free(herois);
	herois = NULL;
}

void destroi_bases(base *bases) {
	for(int i = 0; i < N_BASES; i++) {
		fila_destroi(bases[i].espera);
		cjto_destroi(bases[i].presentes);
	}
	free(bases);
	bases = NULL;
}

void destroi_missoes(missao *missoes) {
	for(int i = 0; i < N_MISSOES; i++) {
		cjto_destroi(missoes[i].habilidades);
	}
	free(missoes);
	missoes = NULL;
}

void destroi_mundo(mundo *mundo) {
	destroi_herois(mundo->herois);
	destroi_bases(mundo->bases);
	destroi_missoes(mundo->missoes);
	free(mundo->base_missao);
	free(mundo->fila_max);
	free(mundo);
	mundo = NULL;
}