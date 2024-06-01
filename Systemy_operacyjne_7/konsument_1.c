#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#ifndef SEMAFORY_BIBLIOTEKA
#include "semafory_biblioteka.h"
#define SEMAFORY_BIBLIOTEKA 0
#endif

#ifndef PAMIEC_DZIELONA_BIBLIOTEKA
#include "pamiec_dzielona_biblioteka.h"
#define PAMIEC_DZIELONA_BIBLIOTEKA 0
#endif

#ifndef CW_7_H
#include "cw_7.h"
#define CW_7_H 0
#endif

int end_of_data(const char* buffer) {
    for (int i = 0; i < N_ELE; ++i) {
        if (buffer[i] == '\0') {
            return i;
        }
    }
    return -1;
}


int main(int argc, char* argv[])
{
    if (argc != 5) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char* message;
    int length;
    int bytes_written;
    char buffer[N_ELE];
    int destination_des, sh_mem_des;
    sem_t* sem_address_prod;
    sem_t* sem_address_kons;

    if ((destination_des = open(argv[4], O_WRONLY | O_CREAT, 0644)) == -1) {
        perror("open destination file error");
        exit(EXIT_FAILURE);
    }

    sem_address_prod = semaphore_open(argv[2], 0);
    sem_address_kons = semaphore_open(argv[3], 0);
    sh_mem_des = sh_mem_open(argv[1], O_RDWR, 0644);

    SegmentPD* wpd = (SegmentPD*) sh_mem_mmap(NULL, sizeof(SegmentPD), PROT_READ | PROT_WRITE, MAP_SHARED, sh_mem_des, 0);

    while(1) {

        if (bytes_written == 0) {
            break;
        }
        // Kod konsumenta
        sem_wait(sem_address_kons); // Opuszczenie semafora Konsumenta

        // Odczytanie porcji danych z pamięci dzielonej
        strncpy(buffer, wpd->bufor[wpd->wyjmij], N_ELE);
        wpd->wyjmij = (wpd->wyjmij + 1) % N_BUF;

        printf("Wartość semafora producenta: %d, wartość semafora konsumenta: %d\n", semaphore_getvalue(sem_address_prod), semaphore_getvalue(sem_address_kons));
        printf("Indeks wyjmowania: %d\n", wpd->wyjmij);
        printf("Liczba zapisanych bajtów: %d\n", bytes_written);
        printf("%s\n", buffer);
        
        if (end_of_data(buffer) !=- 1) {
            if (end_of_data(buffer)==0) {
                break;
            }
            int j = N_ELE - end_of_data(buffer)-1;
            bytes_written = write(destination_des, buffer, j);
            break;
        }

        sem_post(sem_address_prod); // Podniesienie semafora Producenta

        bytes_written = write(destination_des, buffer, N_ELE);

        // Przetwarzanie danych
        sleep(1);
    }

    if (close(destination_des) == -1) {
        perror("close destination file error");
        exit(EXIT_FAILURE);
    }

    if (sh_mem_munmap(wpd, sizeof(SegmentPD)) == -1) {
        perror("munmap error");
        exit(EXIT_FAILURE);
    }

    if (close(sh_mem_des) == -1) {
        perror("close shared memory descriptor error");
        exit(EXIT_FAILURE);
    }

    _exit(EXIT_SUCCESS);
}