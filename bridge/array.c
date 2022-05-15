#include "array.h"

int rand_in_range(int min, int max)
{
    return rand() % (max-min+1) + min;
}

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
int *create_rand_array(int *array_size)
{

    *array_size = rand_in_range(MIN_ARRAY_SIZE, MAX_ARRAY_SIZE);
    int *array = malloc(sizeof(int) **array_size);

    for (int i = 0; i < *array_size; i++)
        array[i] = rand_in_range(MIN_RAND, MAX_RAND);

    return array;
}

void bubble_sort(int array[], int array_size)
{
    for(int i = 0; i< array_size - 1; i++){
            for(int j = 0; j< array_size - 1; j++){
                if(array[j] > array[j+1])
                    swap(&array[j], &array[j+1]);
            }
    }
}
