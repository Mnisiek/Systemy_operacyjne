# Plik Makefile do kompilacji i wywoływania programów.
###############################################################################

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Domyślne nazwy plików, z których odczytujemy i zapisujemy dane.
WEJSCIE = wejscie.txt
WYJSCIE = wyjscie.txt

# Porównanie zawartości plików
compare:
	diff -s $(WEJSCIE) $(WYJSCIE)

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME4 = cw_4

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC4 = $(NAME4).x

# Zmienne zawierające nazwy tzw. object files.
OBJS4 = $(NAME4).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS4 = 

# Pliki naglowkowe
HEADS4 = 

# Opcje kompilatora i linkera
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -Wall -std=c99 -pedantic

# Włączenie biblioteki matematycznej
LDLIBS = -lm

# Kompilator i linker
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<


.PHONY: all
all: $(EXEC4)

$(EXEC4): $(OBJS4) $(LIBS4) 
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_4

run-cw_4: $(EXEC4)
	./$(EXEC4) $(WEJSCIE) $(WYJSCIE)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )