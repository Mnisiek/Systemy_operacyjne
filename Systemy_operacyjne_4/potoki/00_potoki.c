#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK 16

int main() {

    char buffer[CHUNK];
    ssize_t bytes_read;

    ssize_t source  = open("./test.txt", O_RDONLY);
    bytes_read = read(source, buffer, CHUNK);

    printf ("Odczytano %zd bajtow.\n", bytes_read);
    printf ("Odczytane dane:\n%s\n", buffer);

    close(source);
    exit (EXIT_SUCCESS);

}
