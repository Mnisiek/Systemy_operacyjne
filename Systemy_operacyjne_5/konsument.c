#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define N_WRITE 6
#define N_INFO 14

/*
Proces wykonuje zadanie Konsumenta - odczytuje dane z potoku (FIFO),
a następnie zapisuje je do pliku.
*/

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Błąd, podano niepoprawną liczbę argumentów.\n");
        exit(EXIT_FAILURE);
    }

    char buffer1[N_WRITE];
    char buffer2[N_INFO];
    char* message;
    int length;
    int bytes_read, bytes_written;
    int fifo_des, destination_des; // deskryptory plików

    if ((fifo_des = open(argv[1], O_RDONLY)) == -1) {
        perror("open FIFO error");
        exit(EXIT_FAILURE);
    }

    if ((destination_des = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("open destination file error");
        exit(EXIT_FAILURE);
    }

    // pętla odczytująca dane z potoku
    while ((bytes_read = read(fifo_des, buffer1, N_WRITE)) != 0) {
        if (bytes_read == -1) {
            perror("reading FIFO error");
            exit(EXIT_FAILURE);
        }
        // program czeka losową liczbę sekund (z przedziału 0 - 2)
        sleep(rand() % 3);
        message = "\nOdczytano z potoku: ";
        length = strlen(message);
        write(STDOUT_FILENO, message, length);
        write(STDOUT_FILENO, buffer1, bytes_read);
        sprintf(buffer2, " (%d bajtów)", bytes_read);
        write(STDOUT_FILENO, buffer2, N_INFO);

        if ((bytes_written = write(destination_des, buffer1, bytes_read)) == -1) {
            perror("writting file error");
            exit(EXIT_FAILURE);
        }
        sleep(rand() % 3);
        message = "\nZapisano do pliku: ";
        length = strlen(message);
        write(STDOUT_FILENO, message, length);
        write(STDOUT_FILENO, buffer1, bytes_written);
        sprintf(buffer2, " (%d bajtów)", bytes_written);
        write(STDOUT_FILENO, buffer2, N_INFO);
    }

    message = "\nZapisano całą zawartość potoku\n";
    length = strlen(message);
    write(STDOUT_FILENO, message, length);

    // zamknięcie otwartych deskryptorów pliku i potoku
    if (close(destination_des) == -1) {
        perror("close destination file error");
        exit(EXIT_FAILURE);
    }
    if (close(fifo_des) == -1) {
        perror("close FIFO reading end error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
