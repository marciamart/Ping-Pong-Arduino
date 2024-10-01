#include "kernel.h"

#define POOL_SIZE 10
#define MIN_INT -30000

volatile int start, end;

volatile static process* pool[POOL_SIZE];

char kernelAddProc(process* func){
    if(((end+1)%POOL_SIZE) != start){
        func->deadline += func->period;
        pool[end] = func;
        end = (end+1)%POOL_SIZE;
        return SUCCESS;
    }
    return FAIL;
}

char kernelInit(void){
    start=0;
    end=0;
    return SUCCESS;
}

void kernelLoop(void){
    unsigned int count;
    unsigned int next;
    process* tempProc;
    for(;;){
        if(start != end){
            count = (start+1)%POOL_SIZE;
            next = start;
            while(count != end){
                if((pool[count]->deadline) < (pool[next]->deadline)){
                    next = count;
                }
                count = (count+1)%POOL_SIZE;
            }
            
            tempProc = pool[next];
            pool[next] = pool[start];
            pool[start] = tempProc;

            while(pool[start]->deadline > 0){
                //coloca a cpu em modo de economia de energia
            }
            
            switch (pool[start]->func())
            {
            case REPEAT:
                kernelAddProc(pool[start]);
                break;
            case FAIL:
                break;
            default:
                break;
            }

            start = (start+1)%POOL_SIZE;
        }
    }
}

void kernelTick(void){
    int proc;
    proc = start;
    while(proc!=end){
        if((pool[proc]->deadline)>(MIN_INT)){
            pool[proc]->deadline--;
        }

        proc = (proc+1)%POOL_SIZE;
    }
}

char test1(void){
    return SUCCESS;
}

char test2(void){
    return SUCCESS;
}

char test3(void){
    return SUCCESS;
}