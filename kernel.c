#include "kernel.h"

// int main(void){
//     kernelInit();
//     kernelLoop();
// }

char kernelInit(){
    start = 0;
    end = 0;
    clock_tick = 5;
    linha = 0;
    circular_buffer.num_queues = 0;
    circular_buffer.process_pool = processes;
    input = fopen("input.txt", "r");
    
    if (input == NULL) {
        printf("Erro ao abrir o arquivo.");
        return FAIL;
    }
    return SUCCESS;
}

char exec(void){
    printf("Executado");
    return SUCCESS;
}

char kernelAddProc()
{
    fscanf(input, "%d %d %d", &processes[end].id, &processes[end].priority, &processes[end].time_left);
    return SUCCESS;
}

char kernelRemoveProc(){
    processes[start].priority = 0;
    if(end != start){
    start = (start+1)%MAX_PROCESSES;
    return SUCCESS;
  }
  return FAIL;
}

void kernelLoop(void){
    switch ()
    {
    case /* constant-expression */:
        circular_buffer.exec = priorityScheduling;
        while(!feof(input)){
        if (((end + 1) % MAX_PROCESSES) != start)
        {
            kernelAddProc();
            end = (end + 1) % (MAX_PROCESSES);  
        }else{
            circular_buffer.exec();
        }
        }
        while(circular_buffer.exec() != FAIL)
        {
            processes[start-1].priority = 0;
        }
        fclose(input);
        break;
    
    default:
        break;
    }
circular_buffer.exec = priorityScheduling;
    while(!feof(input)){
        if (((end + 1) % MAX_PROCESSES) != start)
        {
            kernelAddProc();
            end = (end + 1) % (MAX_PROCESSES);  
        }else{
            circular_buffer.exec();
        }
    }
    while(circular_buffer.exec() != FAIL)
    {
        processes[start-1].priority = 0;
    }
    fclose(input);

}
char priorityScheduling(void)
{
    ///@todo consertar o indice de maior prioridade quando entrar no segundo while
    int end_aux = start;
    Process highestPriorityProcess = processes[start];
    while (((end_aux + 1) % MAX_PROCESSES) != start)
    {
        if (processes[end_aux].priority > highestPriorityProcess.priority && processes[end_aux].priority > 0) {
            highestPriorityProcess = processes[end_aux];
            processes[end_aux] = processes[start];
            processes[start] = highestPriorityProcess;
        } 
        end_aux = (end_aux + 1) % (MAX_PROCESSES);
    }
    if(highestPriorityProcess.priority != 0)
        printf("%d. Process ID: %d \tPRIORITY: %d\tTIME LEFT: %d\n", linha++, processes[start].id, processes[start].priority, processes[start].time_left);
    if(kernelRemoveProc() == SUCCESS) return SUCCESS;
    else return FAIL;
}