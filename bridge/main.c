#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "array.h"

void town(){
        int array_size;
        int *array = create_rand_array(&array_size);
        bubble_sort(array, array_size);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    return 0;
}