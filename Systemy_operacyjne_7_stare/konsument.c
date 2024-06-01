
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
#define NELE 20
#define NBUF 10

typedef struct
{
    char bufor[NBUF][NELE];
    int insert;
    int get;
} SegmentSHM;

int ifzero(SegmentSHM SHM, int bufornumber, int* endposition)
{
    int i;
    for (i = 0; i < NELE; i++)
    {
        if (SHM.bufor[bufornumber][i] == '\0')
        {
            *endposition = i;
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{

    /*sprawdzanie liczby argumentów*/
    if (argc != 5)
    {
        printf("Wrong number of arguments (expected: producent semaphore, konsument semaphore, memory and destination file names)\n");
        exit(EXIT_FAILURE);
    }

    /*tworzenie i przypisywanie zmiennych*/
    char *prod_semaph = argv[1];
    char *kons_semaph = argv[2];
    char *shm_name = argv[3];
    char *destfile = argv[4];
    int prodsemvalue, konssemvalue, fdst, bytesread, shmdesc, endposition;

    /*otwarcie pliku do zapisu*/
    fdst = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fdst == -1)
    {
        perror("Open error\n");
        exit(-1);
    }

    /*otwarcie semaforów i pamieci dzielonej, rzutowanie*/
    sem_t *prod_semaph_address = semaphore_open(prod_semaph, 0);
    printf("Producent semaphore's address: %d\n", prod_semaph_address);
    sem_t *kons_semaph_address = semaphore_open(kons_semaph, 0);
    printf("Konsument semaphore's address: %d\n", kons_semaph_address);
    shmdesc = my_shm_open(shm_name);

    SegmentSHM *SHM = (SegmentSHM *)my_mmap(sizeof(SegmentSHM), shmdesc);

    printf("Konsument: Shared memory size:%d\n", sizeof(SegmentSHM));
    printf("Shared memory address:%d\n", shmdesc);

    SHM->get = 0;

    /*zapisywanie wyprodukowanych produktów z pamieci dzielonej do pliku*/
    do
    {
        /*opuszczenie semafora konsumenta*/
        semaphore_post(kons_semaph_address);

        /*wypisywanie wartosci semaforow*/
        prodsemvalue = semaphore_getvalue(prod_semaph_address);
        konssemvalue = semaphore_getvalue(kons_semaph_address);
        printf("\n\nProducent semaphore value: %d\n", prodsemvalue);
        printf("\n\nKonsument semaphore value: %d\n", konssemvalue);

        /*czytanie z pliku do pamięci dzielonej*/
        if (ifzero(*SHM, SHM->get, &endposition) == 0)
        {
            bytesread = write(fdst, SHM->bufor[SHM->get], endposition);
            if (bytesread == -1)
            {
                perror("read error\n");
                _exit(EXIT_FAILURE);
            };
        }
        else
        {
            bytesread = write(fdst, SHM->bufor[SHM->get], NELE);
            if (bytesread == -1)
            {
                perror("read error\n");
                _exit(EXIT_FAILURE);
            }
        }

        /*wypisanie towaru na ekran*/
        printf("Current bufor element index in konsument process: %d\n", SHM->get);
        printf("Bytes read: %d\n", bytesread);
        printf("Text recieved:");
        if (-1 == write(STDOUT_FILENO, SHM->bufor[SHM->get], bytesread))
        {
            perror("write error\n");
            _exit(EXIT_FAILURE);
        }

        /*zwiększenie indeksu*/
        SHM->get = (SHM->get + 1) % NBUF;

        /*podniesienie semafora producenta*/
        semaphore_wait(prod_semaph_address);

    } while (ifzero(*SHM, SHM->get - 1, &endposition));

    /* zamykanie pliku, semaforów i pamięci dzielonej */
    if (close(fdst) == -1)
    {
        perror("Close error\n");
        _exit(EXIT_FAILURE);
    }
    semaphore_close(prod_semaph_address);
    semaphore_close(kons_semaph_address);
    my_close(shmdesc);

    printf("Konsument: End - exiting\n");

    _exit(EXIT_SUCCESS);
}