#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("location of code : %p\n", main);
    printf("location of heap : %p\n", malloc(100e6));
    int x = 3;
    int y = 3;
    printf("location of stack x: %p\n", &x);
    printf("location of stack y: %p\n", &y);
    return 0;
}
