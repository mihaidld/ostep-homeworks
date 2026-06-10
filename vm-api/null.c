#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // int *p = NULL;
    // printf("%d\n", *p);

    // int *x = malloc(sizeof(int));
    // return 0;

    // int *data = malloc(100 * sizeof(int));
    // data[100] = 0;

    // int *data = malloc(100 * sizeof(int));
    // data[0] = 3;
    // printf("value is %d\n", data[0]);
    // free(data);
    // printf("value after free is %d\n", data[0]);

    // int *data = malloc(100 * sizeof(int));
    // printf("data is %p, middle pointer is %p\n", data, data + 50);
    // free(data + 50);

    int *data = malloc(sizeof(int));
    data[0] = 3;
    data = realloc(data, 10 * sizeof(int));
    data[9] = 5;
    data = realloc(data, 20 * sizeof(int));
    data[19] = 10;
    printf("values are: %d %d %d\n", data[0], data[9], data[19]);
    free(data);
}
