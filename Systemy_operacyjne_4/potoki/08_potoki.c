#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK 16

int main() {

    char buffer[CHUNK], buffer2[CHUNK];
    ssize_t bytes_read, bytes_written;
    ssize_t pipe_bytes_read, pipe_bytes_written;

    int despipe[2];

    pipe(despipe);

    printf ("Otwarto potok nienazwany z deskryptorami: %d %d\n", despipe[0], despipe[1]);

    ssize_t source = open("./test.txt", O_RDONLY);
    while ((bytes_read = read(source, buffer, CHUNK))) {
        printf ("Odczytano %zd bajtow z pliku.\n", bytes_read);
        pipe_bytes_written = write(despipe[1], buffer, bytes_read);
        printf ("Przeslano %zd bajtow do potoku.\n", pipe_bytes_written);
        sleep(1);
    }

    close(despipe[1]);

    printf ("\n=========Przesylanie do potoku zakonczone!=========\n\n");

    ssize_t dest = open("./test_copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    while(( pipe_bytes_read = read(despipe[0], buffer2, CHUNK) )) {
        printf ("Odczytano %zd bajtow z potoku.\n", pipe_bytes_read);
        bytes_written = write(dest, buffer2, pipe_bytes_read);
        printf ("Zapisano %zd bajtow.\n", bytes_written);
        sleep(1);
    }

    close(despipe[0]);
    close(source);
    close(dest);
    exit (EXIT_SUCCESS);

}
