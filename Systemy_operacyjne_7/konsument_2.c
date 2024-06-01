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
        sem_wait(sem_address_kons); // Opuszczenie semafora Konsumenta

        // Odczytanie porcji danych z pamięci dzielonej
        strncpy(buffer, wpd->bufor[wpd->wyjmij], N_ELE);
        wpd->wyjmij = (wpd->wyjmij + 1) % N_BUF;
        
        int j = end_of_data(buffer);
        if (j != - 1) { // 0 1 2 |3| -> zwraca 3
            exit(EXIT_FAILURE);
        } else {
            bytes_written = write(destination_des, buffer, j);
        }

        sem_post(sem_address_prod); // Podniesienie semafora Producenta

        // bytes_written = write(destination_des, buffer, N_ELE);

        printf("Wartość semafora producenta: %d, wartość semafora konsumenta: %d\n", semaphore_getvalue(sem_address_prod), semaphore_getvalue(sem_address_kons));
        printf("Indeks wyjmowania: %d\n", wpd->wyjmij);
        printf("Liczba zapisanych bajtów: %d\n", bytes_written);

        if (write(STDOUT_FILENO, wpd->bufor[wpd->wyjmij], bytes_written) == -1) {
            perror("write to stdout error");
            exit(EXIT_FAILURE);
        }
        printf("%c", '\n');

        if (bytes_written == 0) {
            break;
        }

        sleep((rand() % 2) + 1);
    }

    if (close(destination_des) == -1) {
        perror("close destination file error");
        exit(EXIT_FAILURE);
    }

    sh_mem_munmap(wpd, sizeof(SegmentPD));
    sh_mem_close(sh_mem_des);

    _exit(EXIT_SUCCESS);
}
