#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define MAX_RAND 1000
#define MIN_RAND 10
#define MIN_ARRAY_SIZE 20000
#define MAX_ARRAY_SIZE 30000

int rand_in_range(int min, int max);
int *create_rand_array(int *m);
void bubble_sort(int array[], int m);
void swap();