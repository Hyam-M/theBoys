#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"
#include "entidades.h"
#include "defines.h"
#include "simulacao.h"
#include "quicksort.h"

/******************************** Funções auxiliares EventoMissao ********************************************/

/* Ordena de forma crescente os id's das bases de acordo com a distância entre
a base e a missão */
void MissaoOrdenaDistancias(int vet[], mundo *m, struct missaoItem *item) {
    int n = m->Nbases;
    baseDist *aux = malloc(n * sizeof(baseDist));

    /* Calcula a distância de cada base até a missão */
    for (int i = 0; i < n; i++) {

        int dx = m->bases[i].local.x - item->m->local.x;
        int dy = m->bases[i].local.y - item->m->local.y;

        aux[i].id   = m->bases[i].id;
        aux[i].dist = sqrt(dx*dx + dy*dy);
    }

    /* ordena o vetor aux de acordo com o campo .dist usando quicksort */
    quickSort(aux, 0, m->Nbases - 1);

    /* Preenche o vetor de saída com os ID's ordenados */
    for (int i = 0; i < n; i++) {
        vet[i] = aux[i].id;
    }

    free(aux);
}

/* retorna 1 se a base apta, e 0 caso contrário */
int MissaoBaseApta(int id, mundo *m, struct missaoItem *item, struct cjto_t **conjHabilidadeApto)
{
    struct cjto_t *conjHabilidade = cjto_cria(m->Nhabilidades);
    struct cjto_t *aux = NULL;

    for (int k = 0; k < m->Nherois; k++) {
        if (cjto_pertence(m->bases[id].presentes, k)) {
            aux = conjHabilidade;
            conjHabilidade = cjto_uniao(m->herois[k].habilidades, conjHabilidade);
            cjto_destroi(aux);
        }
    }

    /* Se base é apta */
    if (cjto_contem(conjHabilidade, item->m->habilidades)) {
        /* libera conjunto anterior se existir */
        if (*conjHabilidadeApto != NULL)
            cjto_destroi(*conjHabilidadeApto);
        /* guarda a cópia atual */
        *conjHabilidadeApto = cjto_copia(conjHabilidade);

        cjto_destroi(conjHabilidade);
        return 1;
    }

    /* base não apta */
    cjto_destroi(conjHabilidade);
    return 0;
}

/* retorna o id do herói com mais xp de determinada base */
int MissaoHeroiMaisXP(base *b, mundo *m) {
    int melhor_id = -1;
    int melhor_xp = -1;

    for (int i = 0; i < m->Nherois; i++) {
        if (cjto_pertence(b->presentes, i) && m->herois[i].status == 1 && m->herois[i].xp > melhor_xp) {
            melhor_xp = m->herois[i].xp;
            melhor_id = i;
        }
    }
    return melhor_id;
}

/*incrementa as estátisticas e libera oitem*/
void MissaoCumprida(base *apta, mundo *m, struct missaoItem *item, struct cjto_t **conjHabilidadeApto) {
    /* procura os hérois que estão  */
        for (int k = 0; k < m->Nherois; k++) {
            if (cjto_pertence(apta->presentes, k)) {
                m->herois[k].xp++;
            }
        }
        /* +1 missao cumprida */
        m->missoes_cumpridas++;
        /* incrementa o missao comprida por base */
        m->base_missao[apta->id]++;

        /* salva a tentativa max e min */
        if (item->tentativa < m->tent_min) {
            m->tent_min = item->tentativa;
        } else if (item->tentativa > m->tent_max) {
            m->tent_max = item->tentativa;
        }

        cjto_destroi(*conjHabilidadeApto);
        free(item);
}

/***************************** Funções EVENTOS *******************************/
void EventoChega(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }
    /* +1 evento tratado */
    m->eventos_tratados++;

    printf("%6d: CHEGA HEROI %2d BASE %2d (%2d/%2d) ", instante, h->id, b->id, b->presentes->num, b->lotacao);
    /* Atualiza a base do herói */
    h->base = b->id;

    int espera;
    /* Se há vagas em B e a fila de espera em B está vazia */
    if ((b->presentes->num < b->lotacao) && (fila_tamanho(b->espera) == 0) ) {
        espera = 1;
    }
    /* Se o Herói tem paciência para esperar caso tenha fila */
    else
        espera = ((h->paciencia) > (10 * b->espera->num));

    if (espera) {
        printf("ESPERA\n");
        /* cria ESPERA */
        fprio_insere(simulacao, hitem, evESPERA, instante);
    }
    else {
        printf("DESISTE\n");
        /* Cria DESISTE */
        fprio_insere(simulacao, hitem, evDESISTE, instante);
    }
}

void EventoEspera(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }

    /* +1 evento tratado */
    m->eventos_tratados++;

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", instante, h->id, b->id, fila_tamanho(b->espera));
    /* coloca o herói na fila de espera */
    fila_insere(b->espera, h->id);
    /* salva o valor máximo do tamanho da fila de espera */
        if (m->fila_max[hitem->b->id] < b->espera->num) {
            m->fila_max[hitem->b->id] = b->espera->num;
        }
    /* Chama AVISA */
    fprio_insere(simulacao, hitem, evAVISA, instante);
}

void EventoDesiste(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }

    /* +1 evento tratado */
    m->eventos_tratados++;

    printf("%6d: DESISTE HEROI %2d BASE %d\n", instante, h->id, b->id);

    fprio_insere(simulacao, hitem, evVIAJA, instante);
}

void EventoAvisa(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    /* +1 evento tratado */
    m->eventos_tratados++;

    base *b = hitem->b; 

    printf("%6d: AVISA PORTEIRO BASE %2d (%2d/%2d) ", instante, b->id, b->presentes->num, b->lotacao);
    fila_imprime(b->espera);
    // cria ENTRA 
    while(b->presentes->num < b->lotacao && fila_tamanho(b->espera) != 0) {
        int item = fila_retira(b->espera);
        printf("%6d: AVISA PORTEIRO BASE %2d ADMITE %2d\n", instante, b->id, item);
        // insere o herói na base
        cjto_insere(b->presentes, item);
        // Cria um novo item para não dar erro.
        struct heroi_base *copia = malloc(sizeof(struct heroi_base));
        copia->b = hitem->b;
        copia->h = &m->herois[item];
        fprio_insere(simulacao, copia, evENTRA, instante);
    }
    // Preciso liberar o hitem, pq nn vai mais ser usado.
    free(hitem);
}

void EventoEntra(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }
    
    /* +1 evento tratado */
    m->eventos_tratados++;

    // calcula tempo de permanência.
    int tpb = 15 + h->paciencia * (1 + rand() % 21); 
    printf("%6d: ENTRA HEROI %2d BASE %2d (%2d/%2d) SAI %d\n", instante, h->id, b->id, b->presentes->num, b->lotacao, instante+tpb);
    fprio_insere(simulacao, hitem, evSAI, instante + tpb);
}

void EventoSai(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }

    printf("%6d: SAI HEROI %2d BASE %2d (%2d/%2d)\n", instante, h->id, b->id, b->presentes->num, b->lotacao);

    /* +1 evento tratado */
    m->eventos_tratados++;

    // Retira o herói da base;
    cjto_retira(b->presentes, h->id);
    h->base = -1;
    
    // Cria VIAGEM
    fprio_insere(simulacao, hitem, evVIAJA, instante);
    // Cria uma cópia do item - assim não duplica
    struct heroi_base *copia = malloc(sizeof(struct heroi_base));
    copia->h = hitem->h;
    copia->b = hitem->b;
    // Cria AVISA
    fprio_insere(simulacao, copia, evAVISA, instante);
}

void EventoViaja(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_base *hitem = (struct heroi_base *)item;

    heroi *h = hitem->h;
    base *b = hitem->b; 

    /* Se o herói estiver morto, ignora */
    if (h->status == 0) {
        free(hitem);
        return;
    }

    /* +1 evento tratado */
    m->eventos_tratados++;

    /* Caso ele não tenha passado pelo evento de saida, pois desistiu na fila */
    if (cjto_pertence(b->presentes, h->id)) {
        cjto_retira(b->presentes, h->id);
    }

    base *novaBase = &m->bases[rand() % m->Nbases];

    int distancia;
    int duracao;
    // calculo da distância.
    int dx = b->local.x - novaBase->local.x;
    int dy = b->local.y - novaBase->local.y;
    distancia = sqrt(dx * dx + dy * dy);

    // calculo da duração.
    duracao = distancia/h->velocidade;

    // Atualiza base para a nova base.
    hitem->b = novaBase;
    h->base = novaBase->id;
    printf("%6d: VIAJA HEROI %2d BASE %2d BASE %2d DIST %d VEL %d CHEGA %d\n", instante, 
        h->id, b->id, novaBase->id, distancia, h->velocidade, instante + duracao);

    fprio_insere(simulacao, hitem, evCHEGA, instante + duracao);
}

void EventoMorre(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct heroi_missao *mitem = (struct heroi_missao *)item;

    /* +1 evento tratado */
    m->eventos_tratados++;
    /* +1 morto */
    m->num_mortos++;

    heroi *h = mitem->h;
    missao missao = mitem->m; 

    printf("%6d: MORRE HEROI %2d missão %2d\n", instante, h->id, missao.id);
    printf("%d", h->base);
    // Retira o herói da base.
    cjto_retira(m->bases[h->base].presentes, h->id);

    free(mitem);
    
    struct heroi_base *hitem = malloc(sizeof(struct heroi_base));
    hitem->h = h;
    hitem->b = &m->bases[h->base];
    // cria AVISA.
    fprio_insere(simulacao, hitem, evAVISA, instante);
}

void EventoMissao(struct fprio_t *simulacao, int instante, void *item, mundo *m) {
    struct missaoItem *mitem = (struct missaoItem *)item;

    /* incrementa estátisticas */
    mitem->tentativa++;
    m->tent_total++;
    m->eventos_tratados++;

    printf("%6d: MISSAO %4d TENT %d HAB REQ: ", instante, mitem->m->id, mitem->tentativa
    ); cjto_imprime(mitem->m->habilidades); printf("\n");

    /* cria um vetor ordenado das distancias entre a base e a missão */
    int ord[m->Nbases];  
    MissaoOrdenaDistancias(ord, m, mitem);

    /* Procura a base apta mais próxima e o cjto de habilidades dos heróis presentes */
    base *apta = NULL;
    struct cjto_t *conjHabilidadeApto = NULL;
    int i = 0;

    while(i < m->Nbases && apta == NULL) {
        if(MissaoBaseApta(ord[i], m, mitem, &conjHabilidadeApto)) {
            apta = &m->bases[ord[i]];
        }
        i++;
    }

    /* se houver uma base apta */
    if (apta != NULL) {
        printf("%6d: MISSAO %4d CUMPRIDA BASE %d HAB: ", instante, mitem->m->id, apta->id
        ); cjto_imprime(conjHabilidadeApto); printf("\n");

        MissaoCumprida(apta, m, mitem, &conjHabilidadeApto);
        return;
    }

    /* Se não encontou uma base apta, mas é possível utilizar composto V */
    if (m->NcompostosV != 0 && instante % 2500 == 0) {
        printf("%6d: MISSAO %4d CUMPRIDA COMPOSTO V USADO:\n", instante, mitem->m->id);

        /* base apta vira a base mais próxima */
        apta = &m->bases[ord[0]];

        /* id do herói com mais xp da base apta(mais próxima) */
        int maior_xp = MissaoHeroiMaisXP(apta, m);

        /* Cria um evento morte para o herói que usou composto V */
        struct heroi_missao *hitem = malloc(sizeof(struct heroi_missao));
        hitem->h = &m->herois[maior_xp];
        hitem->h->status = 0;
        hitem->m = *mitem->m;
        fprio_insere(simulacao, hitem, evMORRE, instante);

        conjHabilidadeApto = cjto_cria(m->Nhabilidades);
        MissaoCumprida(apta, m, mitem, &conjHabilidadeApto);
        m->NcompostosV--;

        return;
    }

    /* Impossível fazer a missão nesse instante - reagenda */
    fprio_insere(simulacao, mitem, evMISSAO, instante + 24 * 60);
    printf("%6d: MISSAO %d IMPOSSIVEL\n", instante, mitem->m->id);
}

void EventoFim(int instante, mundo *m) {
    printf("%6d: FIM DO MUNDO\n", instante);
    /* Imprime as estátisticas de todos os heróis */
    for (int i = 0; i < m->Nherois; i++) {
        if (m->herois[i].status == 1) {
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS ", i, m->herois[i].paciencia, 
                m->herois[i].velocidade, m->herois[i].xp); cjto_imprime(m->herois[i].habilidades); printf("\n");
        }
        else {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS ", i, m->herois[i].paciencia, 
                m->herois[i].velocidade, m->herois[i].xp); cjto_imprime(m->herois[i].habilidades); printf("\n");
        }
    }

    for (int i = 0; i  < m->Nbases; i++) {
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, m->bases[i].lotacao, m->fila_max[i], m->base_missao[i]);
    }
    double missoes_cumpridas = (double)m->missoes_cumpridas/m->Nmissoes*100;
    double taxa_mortalidade = (double)m->num_mortos/m->Nherois*100;
    double media_tentativas = (double)m->tent_total/m->Nmissoes;
    printf("EVENTOS TRATADOS: %d\n", m->eventos_tratados);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m->missoes_cumpridas, m->Nmissoes, missoes_cumpridas);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", m->tent_min, m->tent_max, media_tentativas);
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);
}