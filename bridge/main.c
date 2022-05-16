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

#define TOWN_A 0
#define TOWN_B 1
#define DIR_R 0
#define DIR_L 1
ticket_vm *tvm;

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
int bridgeCarid = 0;
int aTownCount = 0;
int bTownCount = 0;
int aQueueSize =0;
int bQueueSize =0;
int direction;
char interruptedFlag = 0;

void sigintHandler();

void printTraffic(){
    printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n", aTownCount, aQueueSize, bridgeCarid, bQueueSize,bTownCount);
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

void *car(void *threadid)
{
    long tid = (long)threadid;
    int current_town = TOWN_A;
    while (!interruptedFlag)
    {
        pthread_mutex_lock(&bridge_mutex);
        aTownCount--;
        bridgeCarid = tid;
        direction = 
        printTraffic();
        bridge();
        bridgeCarid = 0;
        bTownCount++;
        printTraffic();
        pthread_mutex_unlock(&bridge_mutex);
        town();
        
        pthread_mutex_lock(&bridge_mutex);
        bridgeCarid = tid;
        bTownCount--;
        printTraffic();
        bridge();
        bridgeCarid = 0;
        aTownCount++;
        printTraffic();
        pthread_mutex_unlock(&bridge_mutex);
        town();    
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
    aTownCount = numOfCars;
    for (long t = 0; t < numOfCars; t++)
    {
        if (pthread_create(&threads[t], NULL, car, (void *)t))
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