# theBoys
Project created as the final activity of the discipline "Programming I" for the Federal University of Paraná (UFPR).

🦸 The Boys – Simulação de Eventos Discretos

Projeto desenvolvido para a disciplina de Programação 1 (CI1001), inspirado na série The Boys. O objetivo é simular um mundo com heróis, bases e missões utilizando o modelo de simulação a eventos discretos.

📌 Descrição

O sistema simula a dinâmica de um ambiente onde:

Heróis possuem habilidades, experiência, velocidade e paciência
Bases possuem capacidade limitada e filas de espera
Missões surgem aleatoriamente exigindo habilidades específicas
Equipes são formadas dinamicamente para cumprir missões

A simulação ocorre ao longo de 525.600 minutos (1 ano), processando eventos em ordem cronológica.

⚙️ Conceitos Utilizados
Simulação de eventos discretos (SED)
Estruturas de dados:
Fila (fila.c)
Fila de prioridade (fprio.c)
Conjunto (conjunto.h)
Algoritmos:
Ordenação com QuickSort (quicksort.c)
Programação modular em C
Gerenciamento de memória dinâmica

🧱 Estrutura do Projeto
.
├── theboys.c          # Programa principal
├── simulacao.c/h      # Controle da simulação
├── eventos.c/h        # Implementação dos eventos
├── entidades.c/h      # Estruturas do mundo (heróis, bases, missões)
├── fila.c/h           # Fila simples (FIFO)
├── fprio.c/h          # Fila de prioridade (LEF)
├── conjunto.h         # TAD conjunto
├── quicksort.c/h      # Ordenação de bases por distância
├── defines.h          # Constantes do sistema
├── makefile           # Compilação
└── theboys.pdf        # Especificação do projeto

🔄 Funcionamento da Simulação

A simulação segue o modelo clássico de eventos discretos:

Inicialização do mundo (heróis, bases, missões)
Criação da Lista de Eventos Futuros (LEF)
Execução do loop principal:
Remove o próximo evento da fila de prioridade
Atualiza o tempo (relógio)
Executa o evento
Gera novos eventos

📅 Eventos Implementados
CHEGA → Herói chega na base
ESPERA → Entra na fila
DESISTE → Vai para outra base
AVISA → Porteiro gerencia fila
ENTRA → Herói entra na base
SAI → Herói sai da base
VIAJA → Deslocamento entre bases
MISSAO → Execução de missão
MORRE → Morte de herói
FIM → Encerramento da simulação

▶️ Como Compilar e Executar
🔧 Compilar
bash: make
▶️ Executar
bash: ./theboys
📊 Saída

Durante a execução, o programa imprime eventos como:
45844: CHEGA HEROI 0 BASE 2 ( 7/ 9) ESPERA
46101: SAI HEROI 30 BASE 2 ( 7/9)
120037: MISSAO 4150 CUMPRIDA BASE 7

Ao final, são exibidas estatísticas como:
. Número de eventos processados;
. Missões cumpridas;
. Heróis mortos;
. Uso de Compostos V;
. Desempenho das bases.

🧠 Destaques da Implementação
Uso de fila de prioridade para gerenciar eventos no tempo
Cálculo de distância entre bases e missões
Seleção da melhor equipe baseada em habilidades
Controle de estado dos heróis (vivo/morto)
Estatísticas detalhadas da simulação
👨‍💻 Autor
Rian Marcos dos Santos Alves
📚 Referência

Projeto baseado na especificação fornecida na disciplina (arquivo theboys.pdf).
Heróis mortos
Uso de Compostos V
Desempenho das bases
