#include <unistd.h> //pause()
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal()
#include <stdlib.h> //exit()

int main() {
	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGTSTP");
		exit(EXIT_FAILURE);
	}
	pause();
	exit(EXIT_SUCCESS);
}

