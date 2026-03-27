#include <stdlib.h>
#include "simulacao.h"
#include "entidades.h"
#include "eventos.h"
#include "fprio.h"
#include "defines.h"

void simulacaoInicializa(struct fprio_t *simulacao, mundo *m) {
    /* Todos os heróis chegam a uma base nos 3 primeiros dias */
    for (int i = 0; i < m->Nherois; i++) {
        /* Primeiro cria o item */
        struct heroi_base *item = malloc(sizeof(struct heroi_base));
        item->h = &m->herois[i];
        item->b = &m->bases[rand() % m->Nbases];
        
        /* Cria o instante nos primeiros 3 dias e o tipo do evento;*/
        int prio = rand() % 4321;

        /* Agora insere o evento na fila */
        fprio_insere(simulacao, item, evCHEGA, prio);
    }

    /* Todas as missões são inicializadas em instantes aleatórias */
    for (int i = 0; i < m->Nmissoes; i++) {
        /* Recebe a missão e inicializa o acumulador de tentativas*/
        struct missaoItem *item = malloc(sizeof(missao));
        item->m = &m->missoes[i];
        item->tentativa = 0;
        
        int prio = rand() % T_FIM_DO_MUNDO+1;

        fprio_insere(simulacao, item, evMISSAO, prio);
    }

    /* Cria o evento "FIM" */
    void *item = malloc(1); /* Fim não recebe nada além do tempo 
    * aloquei o item só para não dar conflito no algoritmo para remover o evento. */
    fprio_insere(simulacao, item, evFIM, T_FIM_DO_MUNDO);
}

void simulacaoLoop(struct fprio_t *simulacao, mundo *m) {
    int tipo;
    int prio;
    void *item;
    
    do {
        item = fprio_retira(simulacao, &tipo, &prio);
        switch (tipo) {
            // Executa CHEGA  
            case evCHEGA: EventoChega(simulacao, prio, item, m); break;
            // Executa ESPERA
            case evESPERA: EventoEspera(simulacao, prio, item, m); break;
            // Executa DESISTE
            case evDESISTE: EventoDesiste(simulacao, prio, item, m); break;
            // Executa AVISA
            case evAVISA: EventoAvisa(simulacao, prio, item, m); break;
            // Executa Entra
            case evENTRA: EventoEntra(simulacao, prio, item, m); break;
            // Executa SAI
            case evSAI: EventoSai(simulacao, prio, item, m); break;
            // Executa VIAJA
            case evVIAJA: EventoViaja(simulacao, prio, item, m); break;
            // Executa MORRE
            case evMORRE: EventoMorre(simulacao, prio, item, m); break;
            // Executa MISSAO
            case evMISSAO: EventoMissao(simulacao, prio, item, m); break;
            // Executa FIM
            case evFIM: {
                EventoFim(prio, m);
                free(item);
            }
        }
    } while (tipo != evFIM);
}

void simulacaoDestroi(struct fprio_t *simulacao) {
    fprio_destroi(simulacao);
}