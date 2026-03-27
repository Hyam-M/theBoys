// programa principal do projeto "The Boys - 2024/2"
// Autor: Rian Marcos dos Santos Alves, GRR 20250921

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "entidades.h"
#include "defines.h"
#include "eventos.h"
#include "simulacao.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main () { 
	srand(0);
  // iniciar o mundo
  mundo *m = inicializa_mundo();
  /* Inicializa a fila de prioridades */
  struct fprio_t *simulacao = fprio_cria(); 
  simulacaoInicializa(simulacao, m);

  // executar o laço de simulação
  simulacaoLoop(simulacao, m);
  
  // destruir o mundo
  simulacaoDestroi(simulacao);
	destroi_mundo(m);
  return (0) ;
}