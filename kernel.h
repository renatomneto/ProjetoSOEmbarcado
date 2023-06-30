#include <stdio.h>
#include <stdlib.h>

FILE *input;

#define SUCCESS 0
#define FAIL 1
#define REPEAT 2
#define MAX_PROCESSES 10
#define MAX_QUEUE_SIZE 100
#define MAX_NUM_QUEUES 10

typedef char (*ptrFunc)(void);

typedef struct {
  int id;
  int priority;
  int time_left;
  int running;
  int done;
} Process;

typedef struct {
    Process *buffer[MAX_PROCESSES];
    int head;
    int tail;
}Queue;

typedef struct {
    Queue *queues[MAX_PROCESSES];
    Process *process_pool;
    int num_queues;
    ptrFunc exec;
} Scheduler;

Scheduler circular_buffer;
Process processes[MAX_PROCESSES];
Queue queues[MAX_PROCESSES];
int start, end, clock_tick, linha;

char priorityScheduling(void);

/// @todo serparar as filas por faixa de prioridade tipo 1-5 6-10 
char multipleQueue(void);

char kernelInit(void);
char kernelAddProc();
char kernelRemoveProc();

void kernelLoop(void);

char exec(void);
