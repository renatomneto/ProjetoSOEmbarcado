#include "kernel.h"

//Main para debugar o projeto no VSCode
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
    state = BATCH;
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

/**
 * @brief atualiza o indice start para que o buffer circular possa inserir 
 * mais processos até que se chegue ao fim dos processos 
 * @return char Retorna se a operação de remoção foi bem sucedida ou não.
 * retornará FALSE quando o buffer circular ja tiver executado todos os processos
 */
char kernelRemoveProc(){
    processes[start].priority = 0;
    if(end != start){
    start = (start+1)%MAX_PROCESSES;
    return SUCCESS;
  }
  return FAIL;
}

/**
 * @brief Loop principal do kernel, funcionando como 'maquina de estados' para 
 * executar os dois algoritmos sequencialmente
 */
void kernelLoop(void){
    switch (state) //identifica o estado
    {
    case BATCH: //algoritmo em Batch
        circular_buffer.exec = priorityScheduling;

        while(!feof(input)){ //equanto não chegar ao fim do arquivo de input
        //preenchendo o buffer circular
        if (((end + 1) % MAX_PROCESSES) != start)
        {
            kernelAddProc(); //adiciona ao buffer o processo
            end = (end + 1) % (MAX_PROCESSES);  //atualiza o indice do buffer
        }else{ //ao encher o buffer começa a execução dos processos
            circular_buffer.exec();
        }
        }
        //execução dos processos restantes do buffer após todos os processos 
        //do arquivo terem sido lidos
        while(circular_buffer.exec() != FAIL)
        {
            //atualiza a prioridade do processo para que ele seja marcado com 'executado'
            //buffer
            processes[start-1].priority = 0;
        }
        fclose(input); //fecha o arquivo de processos
        kernelNextTask(); //muda para o próximo estado, para executar o próximo algoritmo
        break;
    case INTERATIVE:
        
    default:
        break;
    }
    
}
/**
 * @brief Função que muda o estado de execução do kernel para executar todas as
 * tarefas
 * @return char Retorna SUCCESS caso a mudança seja bem sucedida ou
 * FAIL caso não seja
 */
char kernelNextTask(void){
    //verifica o estado atual e muda para o próximo estado
    //como são apenas 2 estados, o if ficou simplificado
    if(state != INTERATIVE){
        state = INTERATIVE;
        return SUCCESS;
    }else{
        state = BATCH;
        return SUCCESS;
    }
    return FAIL;
}

/**
 * @brief Algoritmo Priority-based para escalonar os processos em Batch com base
 * na maior prioridade. Importante ressaltar que os processos são executados 
 * após o buffer circular estar preenchido, e então, quando chega um novo processo, 
 * se ele for de maior prioridade que os processos do buffer, ele é executado.
 * @return char Retorna SUCCESS se o processo foi executado e então removido do buffer,
 * retorna FAIL quando não houver mais processos para executar
 */
char priorityScheduling(void)
{
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