# Projeto de Nano Kernel com algoritmos de escalonamento em lote e interativo
### Disciplina de Sistemas Operacionais Embarcados (ECOS03) ministrada pelo professor Otávio Gomes pela Universidade Federal de Itajubá (Unifei).

Nano Kernel que executa dois algoritmos de escalonamento:
* Priority-based Scheduling 
* Multilevel Queue Scheduling (Priority)

O projeto executa dois loops (simulando uma máquina de estados para um RTOS), em que cada loop executa um algoritmo de escalonamento.

O primeiro loop executa o escalonador em lotes com o algoritmo Priority-based Scheduling, em que o processo de maior prioridade é executado até o seu término.

O segundo loop executa o escalonador interativo de multiplas filas, com quantum = 5 unidades de tempo, onde o algoritmo utilizado foi Multilevel Queue Scheduling (Priority). Foram definidas 3 filas, com variação de prioridade de 7 unidades, ou seja, a fila de menor prioridade possuía processos na faixa de 1 a 7 de prioridade. A maior fila possuía processos na faixa de 15 a 21. A fila de maior prioridade então era executada, onde cada processo teria 5 quantum para executar, então era subtraído 5 do tempo restante do processo e, se seu tempo restante fosse menor que 0, o processo estava então finalizado.

Um vídeo explicativo pode ser encontrado em [aqui.](https://youtu.be/NKNKjauWosQ)
