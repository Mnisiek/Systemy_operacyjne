#include <unistd.h> //pause()
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal()
#include <stdlib.h> //exit()

int main() {
    printf ("Sygnal SIGTSTP jest ignorowany\n");
	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGTSTP");
		exit(EXIT_FAILURE);
	}
	sleep(3);
    printf ("Powrot do domyslnego zachowania dla sygnalu SIGTSTP\n");
	if (signal(SIGTSTP, SIG_DFL) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGTSTP");
		exit(EXIT_FAILURE);
	}
	sleep(3);
	exit(EXIT_SUCCESS);
}

