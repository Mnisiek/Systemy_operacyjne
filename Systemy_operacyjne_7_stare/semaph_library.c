
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

/*
Biblioteka zawierająca funkcje do pracy z semaforami nazwanymi.
*/

sem_t* semaphore_open(const char* name, int oflag)
{
    sem_t* ret_val = sem_open(name, oflag);
    if (ret_val == SEM_FAILED) {
        perror("open semaphore error");
        return ret_val;
    } else {
        return ret_val;
    }
}

sem_t* semaphore_create(const char* name, int oflag, mode_t mode, unsigned int value)
{
    sem_t* ret_val = sem_open(name, oflag, mode, value);
    if (ret_val == SEM_FAILED) {
        perror("create semaphore error");
        return ret_val;
    } else {
        return ret_val;
    }
}

int semaphore_getvalue(sem_t* sem)
{
    int sval;
    if (sem_getvalue(sem, &sval) == -1) {
        perror("get semaphore value error");
        return -1;
    } else {
        return sval;
    }
}

int semaphore_post(sem_t* sem)
{
    if (sem_post(sem) == -1) {
        perror("post semaphore error");
        return -1;
    } else {
        return 0;
    }
}

int semaphore_wait(sem_t* sem)
{
    if (sem_wait(sem) == -1) {
        perror("wait semaphore error");
        return -1;
    } else {
        return 0;
    }
}

int semaphore_close(sem_t* sem)
{
    if (sem_close(sem) == -1) {
        perror("close semaphore error");
        return -1;
    } else {
        return 0;
    }
}

int semaphore_unlink(const char* name)
{
    if (sem_unlink(name) == -1) {
        perror("unlink semaphore error");
        return -1;
    } else {
        return 0;
    }
}