
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


/*
Biblioteka zawierająca funkcje do pracy z pamięcią dzieloną.
*/

int sh_mem_open(const char* name, int opening, mode_t mode)
{
    int sh_mem_des = shm_open(name, opening, mode);
    if (sh_mem_des == -1) {
        perror("open shared memory object error");
        return sh_mem_des;
    } else {
        return sh_mem_des;
    }
}

int sh_mem_create(const char* name, int opening, mode_t mode, unsigned int length)
{
    int sh_mem_des = shm_open(name, O_CREAT | O_EXCL | opening, mode);
    if (sh_mem_des == -1) {
        perror("create shared memory object error");
        return -1;
    } else if (ftruncate(sh_mem_des, length) == -1) {
        perror("set shared memory size error");
        return -1;
    } else {
        return sh_mem_des;
    }
}

int sh_mem_close(int sh_mem_des)
{
    if (close(sh_mem_des) == -1) {
        perror("close shared memory object error");
        return -1;
    } else {
        return 0;
    }
}

int sh_mem_unlink(const char* name)
{
    if (shm_unlink(name) == -1) {
        perror("shared memory object unlink error");
        return -1;
    } else {
        return 0;
    }
}

void* sh_mem_mmap(void* address, size_t length, int prot, int flags, int sh_mem_des, off_t offset)
{
    void* maped_ptr = mmap(address, length, prot, flags, sh_mem_des, offset);
    if (maped_ptr == MAP_FAILED) {
        perror("shared memory object map error");
        return MAP_FAILED;
    } else {
        return maped_ptr;
    }
}

int sh_mem_munmap(void* address, size_t length)
{
    if (munmap(address, length) == -1) {
        perror("shared memory object unmap error");
        return -1;
    } else {
        return 0;
    }
}

