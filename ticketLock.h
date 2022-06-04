#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct ticket_machine{
    int now_serving;
    int next_ticket;
    pthread_mutex_t ticket_fetch_lock;
    pthread_cond_t tvm_release;
} ticket_vm;

int getTicket(ticket_vm *tvm);
ticket_vm *intiTicket_vm();
int accquire(ticket_vm *tvm);
void release();
int getNowServing(ticket_vm *tvm);