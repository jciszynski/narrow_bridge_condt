#include <stdio.h>

#include "ticketLock.h"

ticket_vm *intiTicket_vm()
{
    ticket_vm *tvm = malloc(sizeof(ticket_vm));
    if (tvm == NULL)
        return NULL;
    tvm->next_ticket = 1;
    tvm->now_serving = 1;
    pthread_mutex_init(&tvm->ticket_fetch_lock, NULL);
    return tvm;
}

int getTicket(ticket_vm *tvm)
{
    pthread_mutex_lock(&tvm->ticket_fetch_lock);
    int ticket_number = tvm->next_ticket++;
    pthread_mutex_unlock(&tvm->ticket_fetch_lock);

    return ticket_number;
}

int getNowServing(ticket_vm *tvm)
{
    return tvm->now_serving;
}

void release(ticket_vm *tvm)
{
    tvm->now_serving++;
}

int getQueueSize(ticket_vm *tvm)
{
    return tvm->next_ticket - tvm->now_serving;
}