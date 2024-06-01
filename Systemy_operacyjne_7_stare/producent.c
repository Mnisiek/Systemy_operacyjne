
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

int main(int argc, char *argv[])
{

    /*sprawdzanie liczby argumentów*/
    if (argc != 5)
    {
        printf("Wrong number of arguments (expected: producent semaphore, konsument semaphore, memory and source file names)\n");
        exit(EXIT_FAILURE);
    }

    /*tworzenie i przypisywanie zmiennych*/
    char *prod_semaph = argv[1];
    char *kons_semaph = argv[2];
    char *shm_name = argv[3];
    char *sourcefile = argv[4];
    int prodsemvalue, konssemvalue, fsrc, bytesread, shmdesc;

    /*otwarcie pliku do odczytu*/
    fsrc = open(sourcefile, O_RDONLY, 0666);
    if (fsrc == -1)
    {
        perror("Open error");
        exit(-1);
    }

    /*otwarcie semaforów i pamieci dzielonej, rzutowanie*/
    sem_t *prod_semaph_address = semaphore_open(prod_semaph, 0);
    printf("Producent semaphore's address: %d\n", prod_semaph_address);
    sem_t *kons_semaph_address = semaphore_open(kons_semaph, 0);
    printf("Konsument semaphore's address: %d\n", kons_semaph_address);
    shmdesc = my_shm_open(shm_name);

    SegmentSHM *SHM = (SegmentSHM *)my_mmap(sizeof(SegmentSHM), shmdesc);

    printf("Producent: Shared memory size:%d\n", sizeof(SegmentSHM));
    printf("Shared memory address:%d\n", shmdesc);

    SHM->insert = 0;

    /*zapisywanie wyprodukowanych produktów do pamięci dzielonej*/
    do
    {
        /*opuszczenie semafora producenta*/
        semaphore_run(prod_semaph_address);

        /*wypisywanie wartosci semaforow*/
        prodsemvalue = semaphore_getvalue(prod_semaph_address);
        konssemvalue = semaphore_getvalue(kons_semaph_address);
        printf("\n\nProducent semaphore value: %d\n", prodsemvalue);
        printf("\n\nKonsument semaphore value: %d\n", konssemvalue);

        /*czytanie z pliku do pamięci dzielonej*/
        bytesread = read(fsrc, SHM->bufor[SHM->insert], NELE);
        if (bytesread == -1)
        {
            perror("read error\n");
            _exit(EXIT_FAILURE);
        }
        if (bytesread < NELE)
        {
            SHM->bufor[SHM->insert][bytesread] = '\0';
        }

        /*wypisanie towaru na ekran*/
        printf("Current bufor element index in producent process: %d\n", SHM->insert);
        printf("Bytes read: %d\n", bytesread);
        printf("Text produced:");
        if (-1 == write(STDOUT_FILENO, SHM->bufor[SHM->insert], bytesread))
        {
            perror("write error\n");
            _exit(EXIT_FAILURE);
        }

        /*zwiększenie indeksu*/
        SHM->insert = (SHM->insert + 1) % NBUF;

        /*podniesienie semafora konsumenta*/
        semaphore_wait(kons_semaph_address);

    } while (bytesread == NELE);

    /* zamykanie pliku, semaforów i pamięci dzielonej */
    if (close(fsrc) == -1)
    {
        perror("Close error\n");
        _exit(EXIT_FAILURE);
    }
    semaphore_close(prod_semaph_address);
    semaphore_close(kons_semaph_address);
    my_close(shmdesc);

    printf("Producent: End - exiting\n");

    _exit(EXIT_SUCCESS);
}