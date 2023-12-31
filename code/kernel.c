#include "kernel.h"

/**
 * @brief Inicialização das variaveis de controle do Kernel
 * 
 * @return char Retorna FAIL caso não seja possível abrir o arquivo de input e 
 * retorna SUCCESS caso a inicialização tenha sido bem sucedida
 */
char kernelInit(){
    start = 0; //indice que controla a inserção ou exclusão de itens do buffer circular
    end = 0; //indice que irá percorrer o buffer circular
    clock_tick = 5; //quantum
    linha = 0; //controle numero de linhas
    circular_buffer.num_queues = 0; //controle do numero de filas
    circular_buffer.process_pool = processes; //apontando os processos ao buffer circular
    circular_buffer.queues = queues; //apontando as filas ao buffer circular
    state = BATCH; //estado do sistema (usado para executar os algoritmos)
    //abertura do arquivo com os processos
    input = fopen("input.txt", "r");
    if (input == NULL) { //verificação de erro
        printf("Erro ao abrir o arquivo.");
        return FAIL;
    }
    return SUCCESS;
}

/**
 * @brief Função que adiciona os processo de acordo com o arquivo de entrada 
 * 
 * @return char Retorna SUCCESS se o processo foi corretamente adicionado ou FAIL 
 * caso contrário
 */
char kernelAddProc()
{
    //adiciona ao procesos de indice 'end' as informações da linha do arquivo
    fscanf(input, "%d %d %d", &processes[end].id, &processes[end].priority, &processes[end].time_left);
    if(processes[end].id >= 0)
        return SUCCESS;
    return FAIL;
}

/**
 * @brief atualiza o indice start para que o buffer circular possa inserir 
 * mais processos até que se chegue ao fim dos processos 
 * @return char Retorna se a operação de remoção foi bem sucedida ou não.
 * retornará FALSE quando o buffer circular ja tiver executado todos os processos.
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
 * executar os dois algoritmos sequencialmente.
 */
void kernelLoop(void){
    switch (state) //identifica o estado
    {
    case BATCH: //algoritmo em Batch
        printf("\n================= PRIORITY (BATCH) =================\n");
        circular_buffer.exec = priorityScheduling;

        while(!feof(input)){ //equanto não chegar ao fim do arquivo de input
        //preenchendo o buffer circular
        if (((end + 1) % MAX_PROCESSES) != start)
        {
            kernelAddProc(); //adiciona o processo ao buffer
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

    case INTERATIVE: // adicionará todos os processos do arquivo às filas para então executar
        printf("\n=============== MULTILEVEL BY PRIORITY ==============\n");
        linha = 0;
        input = fopen("input.txt", "r");
        if (input == NULL) {
            printf("Erro ao abrir o arquivo.");
        }
        circular_buffer.exec = multilevelQueuePriority;
        //definindo as filas que terão as prioridades separadas
        kernelAddQueue();
        kernelAddQueue();   
        kernelAddQueue();

        while(!feof(input)){ //equanto não chegar ao fim do arquivo de input
        //preenchendo o buffer circular
        if (((end + 1) % MAX_PROCESSES) != start)
        {
            kernelAddProc(); //adiciona o processo ao buffer
            //definindo a faixa de indices para cada fila
            if(processes[end].priority <= 7)
                kernelEnqueue(circular_buffer.queues, 0); //prioridade de 1 a 7
            else if(processes[end].priority > 7 && processes[end].priority <= 14)
                kernelEnqueue(circular_buffer.queues, 1); //prioridade de 8 a 14
            else if(processes[end].priority > 14 && processes[end].priority <= 21)
                kernelEnqueue(circular_buffer.queues, 2); //prioridade de 15 a 21
            
            end = (end + 1) % (MAX_PROCESSES);  //atualiza o indice do buffer
        }else{
            //atualiza o buffer_circular para continuar adicionando os processos
            kernelRemoveProc();
        }
        }
        //executa o algoritmo até o final
        while(circular_buffer.exec() != SUCCESS);
        fclose(input); //fecha o arquivo de processos
        kernelNextTask(); //muda para o próximo estado, para executar o próximo algoritmo
        break;
    default:
        break;
    }
}

/**
 * @brief Adiciona e inicializa os parametros de uma fila
 * @return char Retorna FAIL se houver o numero máximo de filas ou SUCCESS se a fila foi adicionada
 */
char kernelAddQueue() {
    if (circular_buffer.num_queues >= MAX_NUM_QUEUES) {
        
        return FAIL;
    }
    queues[circular_buffer.num_queues].head = 0;
    queues[circular_buffer.num_queues].tail = 0;
    circular_buffer.num_queues++;
    return SUCCESS; // retorna indice da fila
}

/**
 * @brief insere item no final da fila retorna 0 se sucesso ou -1 se a fila estiver cheia
 * 
 * @param q Fila que terá os processos adicionados
 * @param index Indice da fila de acordo com a faixa de prioridade definida
 * @return char Retorna FAIL se a fila estiver cheia e SUCCESS se o processo foi adicionado 
 * corretamente
 */
char kernelEnqueue(Queue *q, int index) {
    if ((q[index].tail + 1) % MAX_QUEUE_SIZE == q[index].head) {
        return FAIL; 
    }
    q[index].buffer[q[index].tail] = processes[end];
    q[index].tail = (q[index].tail + 1) % MAX_QUEUE_SIZE;
    return SUCCESS;
}

/**
 * @brief Remove item do inicio da fila e retorna um queue_item_t com o ponteiro para os 
 * dados do elemento e o seu tamanho (se fila vazia retorna queue_item_t Null e tamanho 0)
 * @param q Fila que terá os processos retirados
 * @param index Indice da fila de acordo com a faixa de prioridade definida
 * @return char Retorna FAIL se a fila estiver vazia e SUCCESS se ainda não estiver vazia
 */
char kernelDequeue(Queue *q, int index) {
    q[index].head = (q[index].head + 1) % MAX_QUEUE_SIZE;
    if (q[index].head == q[index].tail) {
        return FAIL; 
    }
    return SUCCESS;
}

/**
 * @brief Função que muda o estado de execução do kernel para executar todas as
 * tarefas
 * @return char Retorna SUCCESS caso a mudança seja bem sucedida ou
 * FAIL caso não seja.
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
 * retorna FAIL quando não houver mais processos para executar.
 */
char priorityScheduling(void)
{
    int end_aux = start;//indice auxiliar para percorrer o buffer circular
    Process highestPriorityProcess = processes[start]; //processo auxiliar para encontrar a maior prioridade
    while (((end_aux + 1) % MAX_PROCESSES) != start)
    {
        //se o processo auxiliar tiver menor prioridade que o processo atual no buffer circular, realiza a troca
        //adicionando o processo auxiliar (maior prioridade) à posição 'start' pois será executado e removido 
        //pela função kernelRemoveProc()
        if (processes[end_aux].priority > highestPriorityProcess.priority && processes[end_aux].priority > 0) {
            highestPriorityProcess = processes[end_aux];
            processes[end_aux] = processes[start];
            processes[start] = highestPriorityProcess;
        } 
        end_aux = (end_aux + 1) % (MAX_PROCESSES); //atualiza indice que está percorrendo o buffer
    }
    //imprime o processo a ser executado
    processes[start].time_left = 0; //atualiza o tempo restante de execução
    printf("%d. Process ID: %d \tPRIORITY: %d\tTIME-LEFT: %d\n", linha++, processes[start].id, processes[start].priority, processes[start].time_left);
    //verifica se houve a exclusão de um processo
    if(kernelRemoveProc() == SUCCESS) return SUCCESS;
    else return FAIL;
    
}

/**
 * @brief Algoritmo de escalonamento de multifilas com prioridade. O algoritmo inicia pela fila com maior indice
 * pois é a fila com os processos de maior prioridade. TODOS os algoritmos da fila selecionada são executados para então 
 * a próxima fila ser executada. 
 * 
 * @return char Retorna SUCCESS ao finalizar a execução do algoritmo
 */
char multilevelQueuePriority(void)
{
    //variaveis de controle
    int head_aux, tail_aux, index, i;
    //variavel de controle de execução das filas
    char queueEmpty;
    Process aux; //processo auxiliar
    //enquanto todas as filas não forem removidas
    while(circular_buffer.num_queues > 0){
        index = circular_buffer.num_queues-1;
        head_aux = queues[index].head;
        tail_aux = queues[index].tail-1;
        queueEmpty = REPEAT;

        //executa até a fila estiver vazia
        while(queueEmpty != SUCCESS){
            //for para percorrer todos os processos da fila
            for(i = head_aux; i <= tail_aux; i++){
                //atualiza o status do processo da fila
                queues[index].buffer[i].running = 1;
                //subtrai o clock_tick do tempo do processo
                queues[index].buffer[i].time_left -= clock_tick;
                //se o processo não tiver acabado o tempo de execução e a flag done não estiver em 1
                if(queues[index].buffer[i].time_left <= 0 && queues[index].buffer[i].done != 1){
                    queues[index].buffer[i].done = 1; //atualiza a flag que indica o termino da execução

                    //troca o processo executado com o processo que está na 'head' da fila para executar a 
                    //remoção do processo da fila e continuar a execução
                    aux = queues[index].buffer[i];
                    queues[index].buffer[i] = queues[index].buffer[head_aux];
                    queues[index].buffer[head_aux] = aux;
                    printf("%d. Process ID: %d \tPRIORITY: %d\tTIME LEFT: %d\tDONE STATUS: %d\n",linha++, aux.id, aux.priority, aux.time_left, aux.done);
                    //remove o processo em 'head' e verifica se a fila está vazia
                    //se não estiver vazia atualiza o indice de inicio do for
                    if(kernelDequeue(circular_buffer.queues, index) == FAIL){
                        //se a fila estiver vazia altera o indice das filas em execução
                        circular_buffer.num_queues--;
                        //se a fila estiver vazia atualiza a flag de controle
                        queueEmpty = SUCCESS;
                    }else{
                        //atualiza o indice 'head' do for
                        head_aux = queues[index].head;       
                    }
                }
            }
        }
    }
    return SUCCESS;
}