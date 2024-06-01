
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

/*
Proces wykonuje zadanie Producenta - odczytuje dane z pliku i zapisuje
je do pamięci dzielonej.
*/

int main(int argc, char* argv[])
{
    // nazwa_obiektu_pamięci_dzielonej,
    // nazwa_semafora_producenta, nazwa_semafora_konsumenta,
    // nazwa_pliku_wejściowego
    if (argc != 5) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char* message;
    int length;
    int bytes_read;
    sem_t* sem_address_prod;
    sem_t* sem_address_kons;
    int source_des, sh_mem_des;
    char buffer[N_ELE];

    if ((source_des = open(argv[4], O_RDONLY)) == -1) {
        perror("open source file error");
        exit(EXIT_FAILURE);
    }

    sem_address_prod = semaphore_open(argv[2], 0);
    sem_address_kons = semaphore_open(argv[3], 0);
    sh_mem_des = sh_mem_open(argv[1], O_RDWR, 0644);

    printf("Adres semafora producenta: %p\n", (void*) sem_address_prod);
    printf("Adres semafora konsumenta: %p\n", (void*) sem_address_kons);
    printf("Deskryptor pamięci dzielonej: %d\n", sh_mem_des);

    SegmentPD* wpd = (SegmentPD*) sh_mem_mmap(NULL, sizeof(SegmentPD), PROT_WRITE, MAP_SHARED, sh_mem_des, 0);
    wpd->wstaw = 0;
    wpd->wyjmij = 0;

    // while ((bytes_read = read(source_des, wpd->bufor[wpd->wstaw], N_ELE)) != 0) {
    //     if (bytes_read == -1) {
    //         perror("reading file error");
    //         exit(EXIT_FAILURE);
    //     }

    //     semaphore_wait(sem_address_prod);
    //     printf("Coś tam robi %d\n", bytes_read);

    //     if ((wpd->wstaw + 1) % N_BUF != wpd->wyjmij) {
    //         wpd->wstaw = (wpd->wstaw + 1) % N_BUF;
    //         semaphore_post(sem_address_kons);
    //     }
    //     else {
    //         semaphore_post(sem_address_prod); // Jeśli bufor jest pełny, zwracamy dostęp do semafora
    //     }
    // }

    while ((bytes_read = read(source_des, buffer, N_ELE)) != 0) {
            if (bytes_read == -1) {
                perror("reading file error");
                exit(EXIT_FAILURE);
            } else if (bytes_read < N_ELE) {
                wpd->bufor[wpd->wstaw][bytes_read] = '\0';
            }
            printf("Wartość semafora producenta: %d, wartość semafora konsumenta: %d\n", semaphore_getvalue(sem_address_prod), semaphore_getvalue(sem_address_kons));
            printf("Indeks wstawiania: %d\n", wpd->wstaw);
            printf("Liczba pobranych bajtów: %d\n", bytes_read);
            printf("%s\n", buffer);

            // Sprawdź, czy bufor jest pełny
            if ((wpd->wstaw + 1) % N_BUF == wpd->wyjmij) {
                semaphore_wait(sem_address_prod);
            }

            semaphore_wait(sem_address_prod);
            memcpy(wpd->bufor, buffer, N_ELE);
            wpd->wstaw = (wpd->wstaw + 1) % N_BUF;
            semaphore_post(sem_address_kons);

            if (bytes_read == 0) {
                break;
            }
        }

    // message = "\nOczytano całą zawartość pliku wejściowego\n";
    // length = strlen(message);
    // write(STDOUT_FILENO, message, length);

    // zamknięcie otwartych deskryptorów pliku i pamięci dzielonej
    if (close(source_des) == -1) {
        perror("close source file error");
        exit(EXIT_FAILURE);
    }
    sh_mem_munmap(wpd, N_ELE * N_BUF);
    sh_mem_close(sh_mem_des);
    
    exit(EXIT_SUCCESS);

}
