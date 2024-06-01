#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define N_READ 4
#define N_INFO 14

/*
Proces wykonuje zadanie Producenta - odczytuje dane z pliku i zapisuje
je do potoku nazwanego (FIFO).
*/

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char buffer1[N_READ];
    char buffer2[N_INFO];
    char* message;
    int length;
    int bytes_read, bytes_written;
    int source_des, fifo_des; // deskryptory plików

    if ((source_des = open(argv[2], O_RDONLY)) == -1) {
            perror("open source file error");
            exit(EXIT_FAILURE);
        }

    if ((fifo_des = open(argv[1], O_WRONLY)) == -1) {
            perror("open FIFO error");
            exit(EXIT_FAILURE);
        }

    // pętla odczytująca dane z pliku wejściowego
        while ((bytes_read = read(source_des, buffer1, N_READ)) != 0) {
            if (bytes_read == -1) {
                perror("reading file error");
                exit(EXIT_FAILURE);
            }
            // program czeka losową liczbę sekund (z przedziału 0 - 2)
            sleep(rand() % 3);
            message = "\nOdczytano z pliku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer1, bytes_read);
            sprintf(buffer2, " (%d bajtów)", bytes_read);
            write(STDOUT_FILENO, buffer2, N_INFO);

            if ((bytes_written = write(fifo_des, buffer1, bytes_read)) == -1) {
                perror("writting FIFO error");
                exit(EXIT_FAILURE);
            }
            sleep(rand() % 3);
            message = "\nZapisano do potoku: ";
            length = strlen(message);
            write(STDOUT_FILENO, message, length);
            write(STDOUT_FILENO, buffer1, bytes_written);
            sprintf(buffer2, " (%d bajtów)", bytes_written);
            write(STDOUT_FILENO, buffer2, N_INFO);
        }

        message = "\nOczytano całą zawartość pliku wejściowego\n";
        length = strlen(message);
        write(STDOUT_FILENO, message, length);

        // zamknięcie otwartych deskryptorów pliku i potoku
        if (close(source_des) == -1) {
            perror("close source file error");
            exit(EXIT_FAILURE);
        }
        if (close(fifo_des) == -1) {
            perror("close FIFO writting end error");
            exit(EXIT_FAILURE);
        }
    
    exit(EXIT_SUCCESS);

}
