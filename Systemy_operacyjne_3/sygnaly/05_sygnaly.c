#define _GNU_SOURCE //strsignal()
#include <unistd.h> //pause()
#include <stdio.h>  //printf() & perror()
#include <signal.h> //signal()
#include <stdlib.h> //exit()
#include <string.h> //strsignal()

void signal_handler(int);

int main() {
    printf ("PID: %d\n", getpid());
	if (signal(SIGTSTP, signal_handler) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGTSTP");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGCHLD, signal_handler) == SIG_ERR) {
		perror ("Funkcja signal ma problem z SIGCHLD");
		exit(EXIT_FAILURE);
	}
	pause();
	exit(EXIT_SUCCESS);
}

void signal_handler(int sig) {
    printf ("\nSygnal - %s - przechwycony!\n", strsignal(sig));
    sleep(1);
}
