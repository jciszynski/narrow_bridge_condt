#include <stdio.h>
#include "car.h"

void initCarArray(car *Car, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        Car[i].id = i;
        Car[i].state = DEFAULT_CAR_STATE;
        Car[i].curTicket = 0;
    }
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