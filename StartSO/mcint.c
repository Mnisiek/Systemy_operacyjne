// Standardowe pliki naglowkowe
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Wlasne pliki naglowkowe
#include "funkcja.h"

#define NGEN 10000000  // Liczba generowanych punktow MC
 
/* 
   Prosty program do wyznaczania liczby pi metoda Monte Carlo
   oraz wyliczania calki z funkcji dwoch zmiennych metoda podstawowa MC.
   --------------------------------------------------------------------
   Autor: Wieslaw Placzek,                           Krakow, 09.02.2023
   --------------------------------------------------------------------
*/ 
int main(int argc,char *argv[])
{
  unsigned long i, ir1, ir2, nn = NGEN, np = 0;
  double rr1, rr2, dlogi, delta, pi_mc, sum = 0;

  printf("        -------------------\n");
  printf("        Metoda Monte Carlo:\n"); 
  printf("        -------------------\n");
  printf("1) Wyznaczanie wartosci liczby pi. \n");
  printf("2) Wyliczanie calki funkcji dwoch zmiennych.\n");                   
  printf("-> Generuje %g przypadkow ...\n",(double) nn);
  printf("=============================================\n");
  // Inicjalizacja generatora liczb losowych
  srand(1371);
  // Generowanie liczb losowych i wyliczanie pi metoda MC
  for (i = 1; i <= nn; ++i)
    {
      ir1 = rand();
      ir2 = rand();
      rr1 = ( (double) ir1 )/RAND_MAX;
      rr2 = ( (double) ir2 )/RAND_MAX;
      if (rr1*rr1 + rr2*rr2 <= 1.0)
	np++;
      // Wydruki kontrolne
      dlogi = log10(1.0*i);
      delta = dlogi - floor(dlogi);
      if (!delta)
	{
	  pi_mc = 4.0*np/i;
	  printf("Metoda MC: pi = %g\t i = %8lu\n",pi_mc,i);    
	}
      // Liczenie calki metoda podstawowa MC
      sum += myFun(rr1,rr2);
    }
  // Dokladna wartosc pi
  double pi_dokl = acos(-1.0);
  printf("Dokladnie: pi = %g\n",pi_dokl);    
  printf("===================================================\n");
  printf("Calka funcji %g*[exp(x) - ln(y)] na [0,1]x[0,1]\n",WSP);
  printf("-> Metoda MC: %g\n",sum/nn);
  printf("-> Dokladnie: %g\n",WSP*exp(1.0));
  printf("===================================================\n");
}

