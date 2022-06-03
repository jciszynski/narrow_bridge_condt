#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "car.h"



int carComparator (const void * a, const void * b)
{
    car *_acar = (car*)a;
    car *_bcar = (car*)b;
    int _a = _acar->curTicket;
    int _b = _bcar->curTicket;
    if(_a < _b) return -1;
    else if(_a == _b) return 0;
    else return 1;
}

void initCarArray(car *Car, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        Car[i].id = i;
        Car[i].state = DEFAULT_CAR_STATE;
        Car[i].curTicket = INT_MAX;
    }
}

car *copy(car carToCopy){

    car *new = malloc(sizeof(car));

    new->id = carToCopy.id;
    new->state = carToCopy.state;
    new->curTicket= carToCopy.curTicket;
    return new;
}

car *getCurOnBridge(car *Car, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (Car[i].state == BRIDGE_TO_A || Car[i].state == BRIDGE_TO_B)
            return &Car[i];
    }

    return NULL;
}

int countCar(car *Car, int array_size, int count_state)
{
    int sum = 0;
    for (int i = 0; i < array_size; i++)
    {
        if (Car[i].state == count_state)
            sum++;
    }

    return sum;
}

car *listQueue(car *Car, int array_size, int state)
{
    int size = countCar(Car, array_size, state);
    if (size == 0)
        return NULL;

    car *queue = malloc(sizeof(car)*size);
    int queueCursor = 0;
    for (int i = 0; i < array_size; i++)
    {
        if (Car[i].state == state){
            queue[queueCursor].state = Car[i].state;
            queue[queueCursor].id = Car[i].id;
            queue[queueCursor].curTicket = Car[i].curTicket;
            queueCursor++;
        }

    }

    qsort(queue, size, sizeof(car), carComparator);
    return queue;
}

