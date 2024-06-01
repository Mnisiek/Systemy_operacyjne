
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void *my_mmap(size_t, int);
void my_munmap(void*, size_t);
void my_close(int);
void my_shm_unlink(const char*);
int my_shm_open(const char*);
int my_shm_create(const char*);
void my_ftruncate(int, off_t);