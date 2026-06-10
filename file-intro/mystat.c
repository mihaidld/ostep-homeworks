
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/**
 * To see the metadata for a certain file, we can use the
stat() or fstat() system calls. These calls take a pathname (or file
descriptor) to a file and fill in a stat structure

 * struct stat {
dev_t st_dev; // ID of device containing file
ino_t st_ino; // inode number
mode_t st_mode; // protection
nlink_t st_nlink; // number of hard links
uid_t st_uid; // user ID of owner
gid_t st_gid; // group ID of owner
dev_t st_rdev; // device ID (if special file)
off_t st_size; // total size, in bytes
blksize_t st_blksize; // blocksize for filesystem I/O
blkcnt_t st_blocks; // number of blocks allocated
time_t st_atime; // time of last access
time_t st_mtime; // time of last modification
time_t st_ctime; // time of last status change
};
 */

// usage: ./mystat filename

int main(int argc, char *argv[])
{
    int fd, rc;
    struct stat s;

    // fd = open(argv[1], O_RDWR);
    // assert(fd > -1);
    // rc = fstat(fd, &s);
    rc = stat(argv[1], &s);
    assert(rc == 0);
    /**  file size, number of blocks allocated, reference
    (link) count, and so forth. What is the link count of a directory, as
    the number of entries in the directory changes */
    printf("file size: %d\n", (int)s.st_size);
    printf("number of blocks allocated: %d\n", (int)s.st_blocks);
    printf("reference (link) count: %d\n", (int)s.st_nlink);
    printf("inode number: %d\n", (int)s.st_ino);
    /** https://stackoverflow.com/questions/26616038/how-do-i-print-file-permissions-as-a-string
     * UNIX file permissions are conventionally expressed in octal,
     * corresponding format string is "%o", not "%d". rw-r--r-- is really 0644,
     * but in C language 0644 == 420, for example (the latter is in decimal). */
    printf("protection: %3o\n", s.st_mode & 0777);
    printf("protection: %jo (octal)\n", (uintmax_t)s.st_mode);
    printf("time of last access: %lu\n", (unsigned long)s.st_atime);
    printf("time of last access: %s", ctime(&s.st_atime));
    return 0;
}
