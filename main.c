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
#include <limits.h>

#define SAVECURPOS() printf("\033[s")
#define RESTORECURPOS() printf("\033[u")
#define SETCURPOS(x, y) printf("\033[%d;%dH", y, x)
#define SETSCROLLINGAREA() printf("\033[1;19r")
#define DELETELINE() printf("\033[2K")
#define CLEARTERM() printf("\e[1;1H\e[2J")
#define TERMHOME() printf("\033[ H")

car *carArray;
int arraySize;
ticket_vm *tvm;


char interruptedFlag = 0;
char debugFlag = 0;
void sigintHandler();
void printTraffic();

pthread_cond_t tvm_release = PTHREAD_COND_INITIALIZER;     // zmienna warunkowa, sygnalizująca zwolnienie mostu
pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;  // mutex chroniący dostęp do mostu
pthread_mutex_t printer_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex chroniący dostęp do printera (dzięki temu tylko jeden wątek na raz wypisuje swój stan)

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

void enterTownQueue(long tid, int town)
{
    carArray[tid].curTicket = getTicket(tvm);
    carArray[tid].state = town;
    printTraffic();
}

void enterTown(long tid, int town)
{
    carArray[tid].state = town;
    carArray[tid].curTicket = INT_MAX;
    release(tvm);
    pthread_cond_broadcast(&tvm_release);
    printTraffic();
    town();
}


void enterBridge(long tid)
{
    pthread_mutex_lock(&bridge_mutex);
    while (carArray[tid].curTicket != getNowServing(tvm))
    {
        pthread_cond_wait(&tvm_release, &bridge_mutex);
    }
    carArray[tid].state = BRIDGE_TO_A;
    printTraffic();
    bridge();
    pthread_mutex_unlock(&bridge_mutex);
}

void *car_thread(void *threadid)
{
    long tid = (long)threadid;

    // za pierwszym razem nie chcemy wypisywać stanu kolejek
    carArray[tid].curTicket = getTicket(tvm);

    while (!interruptedFlag)
    {
        enterBridge(tid);

        enterTown(tid, TOWN_B);
        enterTownQueue(tid, TOWN_B_QUEUE);

        enterBridge(tid);

        enterTown(tid, TOWN_A);
        enterTownQueue(tid, TOWN_A_QUEUE);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char *p;
    int numOfCars;

    if (argc == 2 || argc == 3)
    {
        long argv1 = strtol(argv[1], &p, 10);
        if (errno != 0 || *p != '\0' || argv1 > INT_MAX || argv1 < INT_MIN)
        {
            fprintf(stderr, "Invalid argument: %s\n", argv[1]);
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

    signal(SIGUSR1, SIG_IGN);
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
    carArray = malloc(sizeof(car) * numOfCars);
    arraySize = numOfCars;

    initCarArray(carArray, numOfCars);
    printTraffic();
    CLEARTERM();

    SETSCROLLINGAREA();
    for (long t = 0; t < numOfCars; t++)
    {
        if (pthread_create(&threads[t], NULL, car_thread, (void *)t))
        {
            printf("Failed to create thread %ld", t);
            exit(EXIT_FAILURE);
        }

        // delay w celu wymuszenia uruchomienia się wątków w kolejności
        // usleep(10000);
    }

    while (!interruptedFlag)
    {
        sleep(10);
    }

    CLEARTERM();
    printf("Exiting... \n");
    printf("Last ticket: %d\n", getNowServing(tvm));
    free(carArray);
    free(tvm);

    return 0;
}

void sigintHandler(int signum)
{
    interruptedFlag = 1;
}

void printTraffic()
{
    pthread_mutex_lock(&printer_mutex);
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

    if (debugFlag)
    {
        SAVECURPOS();
        fflush(stdout);
        SETCURPOS(0, 21);
        DELETELINE();
        printf("Cars in TOWN A: \t");
        for (int i = 0; i < arraySize; i++)
        {
            if (carArray[i].state == TOWN_A)
                printf("%d ", carArray[i].id);
        }
        printf("\n");
        DELETELINE();
        printf("Cars in TOWN A QUEUE:\t");

        car *inAQueue = listQueue(carArray, arraySize, TOWN_A_QUEUE);
        for (int i = 0; i < countCar(carArray, arraySize, TOWN_A_QUEUE); i++)
        {
            printf("%d ", inAQueue[i].id);
        }
        free(inAQueue);

        printf("\n");
        DELETELINE();
        printf("Cars on bridge: \t");
        if (bridgeCarId != -1)
            printf("%d", bridgeCarId);

        printf("\n");
        DELETELINE();
        printf("Cars in TOWN B QUEUE:\t");
        car *inBQueue = listQueue(carArray, arraySize, TOWN_B_QUEUE);
        for (int i = 0; i < countCar(carArray, arraySize, TOWN_B_QUEUE); i++)
        {
            printf("%d ", inBQueue[i].id);
        }
        free(inBQueue);

        printf("\n");
        DELETELINE();
        printf("Cars in TOWN B: \t");
        for (int i = 0; i < arraySize; i++)
        {
            if (carArray[i].state == TOWN_B)
                printf("%d ", carArray[i].id);
        }
        RESTORECURPOS();
        fflush(stdout);
    }
    pthread_mutex_unlock(&printer_mutex);
}