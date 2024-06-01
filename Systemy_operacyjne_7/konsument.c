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
    for (int i = 0; i < N_ELE; i++) {
        if (buffer[i] == '\0') {
            return i;
        }
    }
    return -1;
}


/*
Proces wykonuje zadanie Konsumenta - odczytuje dane z pamięci dzielonej,
a następnie zapisuje je do pliku.
*/

int main(int argc, char* argv[])
{
    // nazwa_obiektu_pamięci_dzielonej,
    // nazwa_semafora_producenta, nazwa_semafora_konsumenta,
    // nazwa_pliku_wyjściowego
    if (argc != 5) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char* message;
    int length;
    int bytes_written;
    sem_t* sem_address_prod;
    sem_t* sem_address_kons;
    int destination_des, sh_mem_des;
    char buffer[N_ELE];

    if ((destination_des = open(argv[4], O_WRONLY)) == -1) {
        perror("open destination file error");
        exit(EXIT_FAILURE);
    }

    sem_address_prod = semaphore_open(argv[2], 0);
    sem_address_kons = semaphore_open(argv[3], 0);
    sh_mem_des = sh_mem_open(argv[1], O_RDWR, 0644);
    printf("Adres semafora producenta: %p\n", (void*) sem_address_prod);
    printf("Adres semafora konsumenta: %p\n", (void*) sem_address_kons);
    printf("Deskryptor pamięci dzielonej: %d\n", sh_mem_des);

    SegmentPD* wpd = (SegmentPD*) sh_mem_mmap(NULL, sizeof(SegmentPD), PROT_READ, MAP_SHARED, sh_mem_des, 0);

    // while (1) {
    //     semaphore_wait(sem_address_kons);

    //     if (wpd->wyjmij == wpd->wstaw) {
    //         sem_post(sem_address_kons);
    //         break;
    //     }

    //     bytes_written = write(destination_des, wpd->bufor[wpd->wyjmij], N_ELE);
    //     if (bytes_written == -1) {
    //         perror("writing to file error");
    //         exit(EXIT_FAILURE);
    //     }

    //     printf("Coś tam robi %d\n", bytes_written);

    //     wpd->wyjmij = (wpd->wyjmij + 1) % N_BUF;

    //     sem_post(sem_address_prod);
    // }

    while(1) {
        semaphore_wait(sem_address_kons);
        memcpy(buffer, wpd->bufor[wpd->wyjmij], N_ELE);
        wpd->wyjmij = (wpd->wyjmij + 1) % N_BUF;

        if (end_of_data(buffer) != -1) {
            if (end_of_data(buffer) == 0) {
                break;
            }
            int x = N_ELE - end_of_data(buffer) - 1;
            bytes_written = write(destination_des, buffer, x);
            break;
        }
        semaphore_post(sem_address_prod);

        if ((bytes_written = write(destination_des, buffer, N_ELE)) == -1) {
            perror("writing file error");
            exit(EXIT_FAILURE);
        }

        printf("Wartość semafora producenta: %d, wartość semafora konsumenta: %d\n", semaphore_getvalue(sem_address_prod), semaphore_getvalue(sem_address_kons));
        printf("Indeks wyjmowania: %d\n", wpd->wyjmij);
        printf("Liczba zapisanych bajtów: %d\n", bytes_written);
        printf("%s\n", buffer);

    }

    // message = "\nZapisano całą zawartość pamięci dzielonej\n";
    // length = strlen(message);
    // write(STDOUT_FILENO, message, length);

    // zamknięcie otwartych deskryptorów pliku i pamięci dzielonej
    if (close(destination_des) == -1) {
        perror("close destination file error");
        exit(EXIT_FAILURE);
    }
    // sh_mem_munmap(wpd, N_ELE * N_BUF);
    // sh_mem_close(sh_mem_des);

    exit(EXIT_SUCCESS);
}
