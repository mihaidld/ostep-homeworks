#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int memUsageMb = atoi(argv[1]); // 1st argument
    printf("number of Mb memory usage: %d\n", memUsageMb);
    printf("PID %d\n", getpid());
    int size = memUsageMb * 1e6 / 4;
    int *arr = calloc(size, sizeof(int));
    // int arr[size];
    if (!arr)
    {
        exit(1);
    }
    int i = 0;
    int j;
    char c;
    while (1)
    {
        scanf("%c", &c);
        if (c == 'q')
        {
            free(arr);
            break;
        }

        if (i < size)
        {
            j = arr[i];
            i++;
        }
        else
        {
            i = 0;
        }
    }

    return 0;
}