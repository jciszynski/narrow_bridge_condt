#include <stdio.h>

typedef struct car{
    int id;
    int state;
    int curTicket;
} car;

#define TOWN_A          0
#define TOWN_A_QUEUE    1
#define BRIDGE_TO_B     2
#define TOWN_B          3
#define TOWN_B_QUEUE    4
#define BRIDGE_TO_A     5
#define DEFAULT_CAR_STATE TOWN_A_QUEUE

void initCarArray(car *Car, int array_size);
car *getCurOnBridge(car *Car, int array_size);
int countCar(car *Car, int array_size, int count_state);