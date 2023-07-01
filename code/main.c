#include "kernel.h"

int main(void){
    kernelInit();
    kernelLoop();
    /*
    * segundo loop chamado apenas para demonstrar o funcionamento
    * dos dois algoritmos. Em um sistema embarcado, o for(;;) dentro
    * de kernelLoop() iria executar corretamente os dois algoritmos.
    */
    //kernelLoop();
    return 0;
}