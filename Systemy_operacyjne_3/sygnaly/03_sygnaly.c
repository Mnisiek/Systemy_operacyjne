#include <unistd.h> //pause()
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal()
#include <stdlib.h> //exit()

void signal_handler(int);

int main() {
	if (signal(SIGTSTP, signal_handler) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGTSTP");
		exit(EXIT_FAILURE);
	}
	pause();
	exit(EXIT_SUCCESS);
}

void signal_handler(int sig) {
    printf ("\nSygnal SIGTSTP przechwycony!\n");
    sleep(1);
}
