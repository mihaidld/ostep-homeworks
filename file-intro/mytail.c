#include <stdio.h> // fprintf, perror
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> // lseek, read
#include <stdlib.h> // exit, atoi
#include <fcntl.h>  // open
#include <string.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

// https://github.com/xxyzz/ostep-hw/blob/master/39/mytail.c
int main(int argc, char *argv[])
{
    struct stat sb;
    int fd, offset, lines;
    char *pathname = "";

    if (argc != 3 || strlen(argv[1]) <= 1 || argv[1][0] != '-')
    {
        fprintf(stderr, "Usage: %s -<offset> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    lines = atoi(argv[1]);
    lines *= -1; // remove -
    // lines++;
    pathname = argv[2];

    if (stat(pathname, &sb) == -1)
        handle_error("stat");

    if ((fd = open(pathname, O_RDONLY)) == -1)
        handle_error("open");

    // set offset to last char
    if (lseek(fd, -1, SEEK_END) == -1)
        handle_error("lseek");

    char buff[sb.st_size];
    while (lines > 0)
    {
        if (read(fd, buff, 1) == -1)
            handle_error("read");
        if (buff[0] == '\n')
            lines--;                      // found newline
        offset = lseek(fd, -2, SEEK_CUR); // offset jump by 2 before
        if (offset == -1)
            break; // went before start
    }

    if (offset > 0 || lines == 0)
    {
        if (lseek(fd, 2, SEEK_CUR) == -1)
            handle_error("lseek");
    }
    else
    {
        if (lseek(fd, 0, SEEK_SET) == -1)
            handle_error("lseek");
    }
    // memset(void *b, int c, size_t len);
    // writes len bytes of value c (converted to an unsigned char)
    // to the string b.
    memset(buff, 0, sb.st_size);
    if (read(fd, buff, sb.st_size) == -1)
        handle_error("read");

    printf("%s", buff);
    close(fd);

    exit(EXIT_SUCCESS);
}