
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shared_memory_library.h"

void *my_mmap(size_t length, int fd)
{
    void *address = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED)
    {
        perror("my_mmap error");
        exit(-1);
    }
    else
    {
        return address;
    }
}

void my_munmap(void *addr, size_t length)
{
    if (munmap(addr, length) == -1)
    {
        perror("my_munmap error");
        exit(-1);
    }
}

void my_close(int fd)
{
    if (close(fd) == -1)
    {
        perror("my_close error");
        exit(-1);
    }
}

void my_shm_unlink(const char *name)
{
    if (shm_unlink(name) == -1)
    {
        perror("my_shm_unlink error");
        exit(-1);
    }
}

int my_shm_create(const char *name)
{
    int desc = shm_open(name, O_EXCL | O_RDWR | O_CREAT, 0644);
    if (desc == -1)
    {
        perror("my_shm_create error");
        exit(-1);
    }
    return desc;
}

int my_shm_open(const char *name)
{
    int desc = shm_open(name, O_RDWR, 0644);
    if (desc == -1)
    {
        perror("my_shm_open error");
        exit(-1);
    }
    return desc;
}

void my_ftruncate(int fd, off_t length)
{
    if (ftruncate(fd, length) == -1)
    {
        perror("my_ftruncate error");
        exit(-1);
    }
}
