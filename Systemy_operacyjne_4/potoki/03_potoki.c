#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK 16

int main() {

    char buffer[CHUNK];
    ssize_t bytes_read, bytes_written;

    ssize_t source  = open("./test_unicode.txt", O_RDONLY);
    bytes_read = read(source, buffer, CHUNK);

    printf ("Odczytano %zd bajtow.\n", bytes_read);
    printf ("Odczytane dane:\n%s\n", buffer);

    ssize_t dest = open("./test_unicode_copy.txt", O_WRONLY, 0644);
    bytes_written = write(dest, buffer, CHUNK);
    
    printf ("Zapisano %zd bajtow.\n", bytes_written);

    close(source);
    close(dest);
    exit (EXIT_SUCCESS);

}
