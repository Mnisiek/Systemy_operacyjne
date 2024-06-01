
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include "semaph_library.h"
#include "shared_memory_library.h"

const char *my_semaph = "firstSemaph";
const char *my_shm = "memory";

/*utworzenie funkcji obsługiwanych przez atexit*/
void sphremove()
{
    semaphore_unlink(my_semaph);
}

void shmremove()
{
    shm_unlink(my_shm);
}

int main(int argc, char *argv[])
{
    /*sprawdzanie liczby argumentów*/
    if (argc != 3)
    {
        printf("Wrong number of arguments (expected: source and destination files' names)\n");
        exit(EXIT_FAILURE);
    }

    /*tworzenie zmiennych*/
    sem_t *my_semaph_address;
    int shm_desc, id, id2;
    char sfname = argv[2];
    char dfname = argv[3];
    unsigned int sem_init_value = 1;

    /*inicjalizacja semafora*/
    my_semaph_address = semaphore_create(my_semaph, O_CREAT | O_EXCL, 0666, sem_init_value);
    printf("Semaphore address: %d\n", my_semaph_address);
    printf("Semaphore inital value: %d\n", sem_init_value);

    /*inicjalizacja pamięci dzielonej*/
    shm_desc = shm_open(my_shm, O_CREAT | O_EXCL, 0666);
    printf("Shared memory address:%d\n", shm_desc);

    id = fork();

    if (id > 0)
    {
        id2 = fork();
        if (id2 > 0)
        {
            wait(NULL);
        }
        else if (id2 == 0)
        {
            execlp("./konsument.x", "konsument.x", my_semaph, my_shm, dfname, NULL);
            perror("execlp error\n");
        }
        else
        {
            perror("fork error\n");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }
    else if (id == 0)
    {
        execlp("./producent.x", "producent.x", my_semaph, my_shm, sfname, NULL);
        perror("execlp error\n");
    }
    else
    {
        perror("fork error\n");
        exit(EXIT_FAILURE);
    }

    atexit(sphremove);
    atexit(shmremove);

    exit(EXIT_SUCCESS);
}
