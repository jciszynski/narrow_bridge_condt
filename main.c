#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "array.h"
#include "ticketLock.h"
#include "car.h"

car *carArray;
int arraySize;
ticket_vm *tvm;

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
char interruptedFlag = 0;

void sigintHandler();

void printTraffic()
{
    char dirArrows[3];
    car *carOnBridge = getCurOnBridge(carArray, arraySize);
    int bridgeCarId;


    if (carOnBridge == NULL)
    {
        bridgeCarId = -1;
    }
    else if (carOnBridge->state == BRIDGE_TO_A)
    {
        strcpy(dirArrows, "<<");
        bridgeCarId = carOnBridge->id;
    }
    else if (carOnBridge->state == BRIDGE_TO_B)
    {
        strcpy(dirArrows, ">>");
        bridgeCarId = carOnBridge->id;
    }

    int aTownCount = countCar(carArray, arraySize, TOWN_A);
    int aQueueSize = countCar(carArray, arraySize, TOWN_A_QUEUE);
    int bTownCount = countCar(carArray, arraySize, TOWN_B);
    int bQueueSize = countCar(carArray, arraySize, TOWN_B_QUEUE);
    if (bridgeCarId != -1)
        printf("A-%d\t%d>>>\t[%s %d %s]\t<<<%d\t%d-B\n", aTownCount, aQueueSize, dirArrows, bridgeCarId, dirArrows, bQueueSize, bTownCount);
    else
        printf("A-%d\t%d>>>\t[       ]\t<<<%d\t%d-B\n", aTownCount, aQueueSize, bQueueSize, bTownCount);
    fflush(stdout);
}
void town()
{
    int array_size;
    int *array = create_rand_array(&array_size);
    bubble_sort(array, array_size);
    free(array);
}

void bridge()
{
    int array_size;
    int *array = create_rand_array(&array_size);
    bubble_sort(array, array_size);
    free(array);
}

void *car_thread(void *threadid)
{
    long tid = (long)threadid;

    while (!interruptedFlag)
    {

        pthread_mutex_lock(&bridge_mutex);
        carArray[tid].state = BRIDGE_TO_B;
        printTraffic();
        bridge();
        carArray[tid].state = TOWN_B;
        printTraffic();
        pthread_mutex_unlock(&bridge_mutex);

        town();
        carArray[tid].state = TOWN_B_QUEUE;
        printTraffic();
        pthread_mutex_lock(&bridge_mutex);
        carArray[tid].state = BRIDGE_TO_A;
        printTraffic();
        bridge();
        carArray[tid].state = TOWN_A;
        printTraffic();
        pthread_mutex_unlock(&bridge_mutex);

        town();
        carArray[tid].state = TOWN_A_QUEUE;
        printTraffic();
    }
}

int main(int argc, char *argv[])
{
    char *p;
    int numOfCars;
    char debugFlag = 0;

    if (argc == 2 || argc == 3)
    {
        long argv1 = strtol(argv[1], &p, 10);
        if (errno != 0 || *p != '\0' || argv1 > INT_MAX || argv1 < INT_MIN)
        {
            fprintf(stderr, "Invalid asrgument: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        else
            numOfCars = argv1;

        if (argc == 3 && strcmp(argv[2], "-debug") == 0)
            debugFlag = 1;
        else if (argc == 3)
        {
            fprintf(stderr, "Invalid argument: %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }

    signal(SIGINT, sigintHandler);
    if (errno)
    {
        fprintf(stderr, "Signal failure: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    tvm = intiTicket_vm();

    if (tvm == NULL)
    {
        fprintf(stderr, "Cannot allocate memory, exiting ...");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[numOfCars];
    carArray = malloc(sizeof(car) *numOfCars);
    arraySize = numOfCars;

    initCarArray(carArray, numOfCars);
    printTraffic();

    for (long t = 0; t < numOfCars; t++)
    {
        if (pthread_create(&threads[t], NULL, car_thread, (void *)t))
        {
            printf("Failed to create thread %ld", t);
            exit(EXIT_FAILURE);
        }
    }

    while (!interruptedFlag)
    {
        sleep(10);
    }

    printf("Exiting... \n");
    return 0;
}

void sigintHandler(int signum)
{
    interruptedFlag = 1;
}