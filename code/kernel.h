#include <stdio.h>
#include <stdlib.h>

//===========================================================
// INICIALIZAÇÃO DOS ARQUIVOS 
//===========================================================
FILE *input;

//===========================================================
// FLAGS DE SINALIZAÇÃO
//===========================================================

#define SUCCESS 0
#define FAIL 1
#define REPEAT 2

//===========================================================
// DEFINES DE TAMANHO DOS BUFFERS
//===========================================================
#define MAX_PROCESSES 10
#define MAX_QUEUE_SIZE 20
#define MAX_NUM_QUEUES 10

//===========================================================
// PONTEIROS DE FUNÇÃO
//===========================================================
typedef char (*ptrFunc)(void);

//===========================================================
// DEFINIÇÃO DE ESTADOS
//===========================================================
typedef enum {
  BATCH = 0,
  INTERATIVE
}STATES;

//===========================================================
// DEFINIÇÃO DAS ESTRUTURAS 
//===========================================================
typedef struct {
  int id;
  int priority;
  int time_left;
  int running;
  int done;
} Process;

typedef struct {
    Process buffer[MAX_QUEUE_SIZE];
    int head;
    int tail;
}Queue;

typedef struct {
    Queue *queues;
    Process *process_pool;
    int num_queues;
    ptrFunc exec;
} Scheduler;

//===========================================================
// CRIAÇÃO DOS BUFFERS E FILAS
//===========================================================
Scheduler circular_buffer;
Process processes[MAX_PROCESSES];
Queue queues[MAX_PROCESSES];
STATES state; 

//===========================================================
// FLAGS DE CONTROLE
//===========================================================
int start, end, clock_tick, linha;

//===========================================================
// PROTÓTIPOS DAS FUNÇÕES DO KERNEL
//===========================================================
char priorityScheduling(void);
char multilevelQueuePriority(void);
char kernelInit(void);
char kernelAddProc();
char kernelRemoveProc();
void kernelLoop(void);
char kernelNextTask(void);
char kernelDequeue(Queue *q, int index);
char kernelEnqueue(Queue *q, int index);
char exec(void);
char kernelAddQueue(void);