# Plik Makefile do kompilacji i wywoływania programów.
###############################################################################

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Domyślna nazwa obiektu pamięci dzielonej
PAMIEC_DZIELONA = pamiec_dzielona

# Domyślne nazwy semaforów do synchonizacji Producenta i Konsumenta.
SEMAFOR_PROD = semafor_producent
SEMAFOR_KONS = semafor_konsument

# Domyślne nazwy plików, z których odczytujemy i zapisujemy dane.
WEJSCIE = wejscie.txt
WYJSCIE = wyjscie.txt

# Porównanie zawartości plików
compare:
	diff -s $(WEJSCIE) $(WYJSCIE)

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME7 = cw_7
PRODUC = producent_3
KONSUM = konsument_3

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC7 = $(NAME7).x
EXEC_PRODUC = $(PRODUC).x
EXEC_KONSUM = $(KONSUM).x

# Zmienne zawierające nazwy tzw. object files.
OBJS7 = $(NAME7).o
OBJS_PRODUC = $(PRODUC).o
OBJS_KONSUM = $(KONSUM).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS7 = pamiec_dzielona_biblioteka.o semafory_biblioteka.o

# Pliki naglowkowe
HEADS7 = 

# Opcje kompilatora i linkera
CFLAGS = -Wall -std=gnu99 -pedantic
LFLAGS = -Wall -std=gnu99 -pedantic -lrt

# Włączenie biblioteki matematycznej
LDLIBS = -lm

# Kompilator i linker
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<


.PHONY: all
all: $(EXEC7) $(EXEC_PRODUC) $(EXEC_KONSUM)

$(EXEC7): $(OBJS7) $(LIBS7)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_PRODUC): $(OBJS_PRODUC) $(LIBS7)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_KONSUM): $(OBJS_KONSUM) $(LIBS7)
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_7

run-cw_7: $(EXEC7) $(EXEC_PRODUC) $(EXEC_KONSUM)
	./$(EXEC7) ./$(EXEC_PRODUC) ./$(EXEC_KONSUM) $(PAMIEC_DZIELONA) $(SEMAFOR_PROD) $(SEMAFOR_KONS) $(WEJSCIE) $(WYJSCIE)

run-producent: $(EXEC_PRODUC)
	./$(EXEC_PRODUC) $(PAMIEC_DZIELONA) $(SEMAFOR_PROD) $(SEMAFOR_KONS) $(WEJSCIE)

run-konsument: $(EXEC_KONSUM)
	./$(EXEC_KONSUM) $(PAMIEC_DZIELONA) $(SEMAFOR_PROD) $(SEMAFOR_KONS) $(WYJSCIE)

run-producent_konsument: $(EXEC_PRODUC) $(EXEC_KONSUM)
	xterm -hold -title PRODUCENT -e make run-producent &

	xterm -hold -title KONSUMENT -e make run-konsument &

# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR};

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )