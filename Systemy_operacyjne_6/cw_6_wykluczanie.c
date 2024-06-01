
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include "biblioteka.h"


int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    int file_des;
    int value;

    sem_t* semaphore_adress = semaphore_open(argv[2], 0);

    for (int i = 0; i < strtol(argv[1], NULL, 10); ++i) {

        printf("Przed sekcją krytyczną\n");
        printf("PID procesu: %d, wartość semafora: %d\n", getpid(), semaphore_getvalue(semaphore_adress));
        semaphore_wait(semaphore_adress);
        sleep(rand() % 5);


        printf("        W sekcji krytycznej\n");
        printf("        Numer sekcji krytycznej: %d\n", i + 1);
        printf("        PID procesu: %d, wartość semafora: %d\n", getpid(), semaphore_getvalue(semaphore_adress));

        if ((file_des = open(argv[3], O_RDONLY)) == -1) {
            perror("open file error");
            exit(EXIT_FAILURE);
        }
        if (read(file_des, &value, sizeof(int)) == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        printf("        Wartość odczytana z pliku: %d\n", value);

        if (close(file_des) == -1) {
            perror("close file error");
            exit(EXIT_FAILURE);
        }
        ++value;
        sleep(rand() % 5);

        if ((file_des = open(argv[3], O_WRONLY)) == -1) {
            perror("open file error");
            exit(EXIT_FAILURE);
        }
        if (write(file_des, &value, sizeof(int)) == -1) {
            perror("write error");
            exit(EXIT_FAILURE);
        }
        if (close(file_des) == -1) {
            perror("close file error");
            exit(EXIT_FAILURE);
        }
        semaphore_post(semaphore_adress);


        printf("Po sekcji krytycznej\n");
        printf("PID procesu: %d, wartość semafora: %d\n", getpid(), semaphore_getvalue(semaphore_adress));
    }

    printf("%c", '\n');
    semaphore_close(semaphore_adress);

    exit(EXIT_SUCCESS);
}

