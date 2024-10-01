#ifndef KERNEL_H_
#define KERNEL_H_

// códigos de retorno
#define SUCCESS	0
#define FAIL	1
#define REPEAT	2

// declaração de ponteiro de função
typedef char (*ptrFunc)(void);

//definição da estrutura de um processo
typedef struct {
	ptrFunc func;
	int period;
	int deadline;
}process;

//protótipos das funções do kernel
char kernelInit(void);
char kernelAddProc(process* newProc);
void kernelLoop(void);// declaração de ponteiro de função
void kernelTick(void);

#endif