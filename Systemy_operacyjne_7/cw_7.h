
#define N_ELE 10 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define N_BUF 5 // Liczba elementow bufora

// Segment pamieci dzielonej
typedef struct {
char bufor[N_BUF][N_ELE+1]; // Wspolny bufor danych
int wstaw, wyjmij; // Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

