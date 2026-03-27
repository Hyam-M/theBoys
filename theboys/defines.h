#ifndef DEFINES_H
#define DEFINES_H

/* DADOS - MUNDO */
#define T_INICIO  0
#define T_FIM_DO_MUNDO  525600
#define N_TAMANHO_MUNDO  20000
#define N_HABILIDADES  10
#define N_HEROIS  (N_HABILIDADES * 5)
#define N_BASES  (N_HEROIS / 5)
#define N_MISSOES  (T_FIM_DO_MUNDO / 100)
#define N_COMPOSTOS_V  (N_HABILIDADES * 3) 

/* EVENTOS */

#define  evCHEGA 1
#define  evESPERA 2
#define  evDESISTE 3
#define  evAVISA 4 
#define  evENTRA 5
#define  evSAI 6
#define  evVIAJA 7
#define  evMORRE 8
#define  evMISSAO 9
#define  evFIM 10

#endif