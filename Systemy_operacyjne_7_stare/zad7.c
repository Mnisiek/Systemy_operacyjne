
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include <signal.h>
#include "semaph_library.h"
#include "shared_memory_library.h"

#define NELE 20
#define NBUF 10

const char *prod_semaph = "prodSemaph";
const char *kons_semaph = "konsSemaph";
const char *my_shm = "memory";

/*utworzenie funkcji obsługiwanych przez atexit*/
void sphremove()
{
    printf("Zad7: sphremove");
    semaphore_unlink(prod_semaph);
    semaphore_unlink(kons_semaph);
}

void shmremove()
{
    printf("Zad7: shmremove");
    my_shm_unlink(my_shm);
}

/*utworzenie struktury dla PD*/

typedef struct
{
    char bufor[NBUF][NELE];
    int insert;
    int get;
} SegmentSHM;

/*utworzenie segmentu do operacji na nim*/
SegmentSHM SHM;

int main(int argc, char *argv[])
{

    /*sprawdzanie liczby argumentów*/
    if (argc != 3)
    {
        printf("Wrong number of arguments (expected: source and destination files' names)\n");
        exit(EXIT_FAILURE);
    }

    /*tworzenie zmiennych*/
    sem_t *prod_semaph_address, *kons_semaph_address;
    int shm_desc, id, id2;
    char *sfname = argv[1];
    char *dfname = argv[2];
    unsigned int sem_init_value = 1;

    /*inicjalizacja semaforów*/
    prod_semaph_address = semaphore_create(prod_semaph, O_CREAT | O_EXCL, 0666, sem_init_value);
    printf("Producent semaphore's address: %d\n", prod_semaph_address);
    printf("Producent semaphore's inital value: %d\n", sem_init_value);

    kons_semaph_address = semaphore_create(kons_semaph, O_CREAT | O_EXCL, 0666, sem_init_value);
    printf("Konsument semaphore's address: %d\n", kons_semaph_address);
    printf("Konsument semaphore's inital value: %d\n", sem_init_value);

    /*inicjalizacja pamięci dzielonej*/
    shm_desc = my_shm_create(my_shm);
    my_ftruncate(shm_desc, sizeof(SHM));
    printf("Shared memory address:%d\n", shm_desc);
    printf("Zad7: Shared memory size:%d\n", sizeof(SHM));

    id = fork();

    if (id > 0)
    {
        if (signal(SIGINT, sphremove) == SIG_ERR)
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }

        if (signal(SIGINT, shmremove) == SIG_ERR)
        {
            perror("signal error");
            exit(EXIT_FAILURE);
        }
        id2 = fork();
        if (id2 > 0)
        {
            if (signal(SIGINT, sphremove) == SIG_ERR)
            {
                perror("signal error");
                exit(EXIT_FAILURE);
            }

            if (signal(SIGINT, shmremove) == SIG_ERR)
            {
                perror("signal error");
                exit(EXIT_FAILURE);
            }
            wait(NULL);
        }
        else if (id2 == 0)
        {
            execlp("./konsument.x", "konsument.x", prod_semaph, kons_semaph, my_shm, dfname, NULL);
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
        execlp("./producent.x", "producent.x", prod_semaph, kons_semaph, my_shm, sfname, NULL);
        perror("execlp error\n");
    }
    else
    {
        perror("fork error\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, sphremove) == SIG_ERR)
    {
        perror("signal error");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, shmremove) == SIG_ERR)
    {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    atexit(sphremove);
    atexit(shmremove);

    printf("Zad7: End - exiting\n");

    exit(EXIT_SUCCESS);
}
