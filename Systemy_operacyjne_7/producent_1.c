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
    if (argc != 5) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char* message;
    int length;
    int bytes_read;
    char buffer[N_ELE];
    sem_t* sem_address_prod;
    sem_t* sem_address_kons;
    int source_des, sh_mem_des;

    if ((source_des = open(argv[4], O_RDONLY)) == -1) {
        perror("open source file error");
        exit(EXIT_FAILURE);
    }

    sem_address_prod = semaphore_open(argv[2], 0);
    sem_address_kons = semaphore_open(argv[3], 0);
    sh_mem_des = sh_mem_open(argv[1], O_RDWR, 0644);

    SegmentPD* wpd = (SegmentPD*) sh_mem_mmap(NULL, sizeof(SegmentPD), PROT_WRITE, MAP_SHARED, sh_mem_des, 0);
    wpd->wstaw = 0;
    wpd->wyjmij = 0;

    while(1) {
        // Kod producenta
        bytes_read = read(source_des, wpd->bufor[wpd->wstaw], N_ELE);
        if (bytes_read == -1) {
            // Obsługa błędu odczytu
            perror("Błąd odczytu z pliku");
            break;
        }  // Zakończ pętlę w przypadku błędu
        else if(bytes_read < N_ELE){
            wpd->bufor[wpd->wstaw][bytes_read] = '\0';  
        }
        sem_wait(sem_address_prod); // Opuszczenie semafora Producenta
     
        wpd->wstaw = (wpd->wstaw + 1) % N_BUF;

        sem_post(sem_address_kons); // Podniesienie semafora Konsumenta

        // // Przetwarzanie danych
        // printf("Producent - Semafory (prod_sem: %d, kons_sem: %d), Indeks bufora: %d, Bajty: %d\n", semaphore_getvalue(sem_address_prod) , semaphore_getvalue(sem_address_kons), wpd->wstaw, bytes_read);

        printf("Wartość semafora producenta: %d, wartość semafora konsumenta: %d\n", semaphore_getvalue(sem_address_prod), semaphore_getvalue(sem_address_kons));
        printf("Indeks wstawiania: %d\n", wpd->wstaw);
        printf("Liczba pobranych bajtów: %d\n", bytes_read);
        printf("%s\n", buffer);
        
        if (-1 == write(STDOUT_FILENO, wpd->bufor[wpd->wstaw], bytes_read))
            {
                perror("write error\n");
                _exit(EXIT_FAILURE);
            }
        if(bytes_read==0){
                break;
            }
        sleep(1);

    }
    

    message = "\nOczytano całą zawartość pliku wejściowego\n";
    length = strlen(message);
    write(STDOUT_FILENO, message, length);

    if (close(source_des) == -1) {
        perror("close source file error");
        exit(EXIT_FAILURE);
    }

    sh_mem_close(sh_mem_des);
    
    _exit(EXIT_SUCCESS);
}