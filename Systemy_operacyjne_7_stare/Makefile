# Plik Makefile do kompilacji i wywoływania programów.
###############################################################################

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Zmienne przechowujące domyślne wartości argumentów wywołania.
N_PROC = 3
N_CRIT = 2
SEMAPHOR_NAME = semafor
FILE_NAME = numer.txt

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME7 = cw_7
WYKLUCZANIE = cw_7_wykluczanie
#BIBLIOTEKA = biblioteka.c

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC7 = $(NAME7).x
EXEC_WYKLUCZANIE = $(WYKLUCZANIE).x
#EXEC_BIBLIOTEKA = $(BIBLIOTEKA).x

# Zmienne zawierające nazwy tzw. object files.
OBJS7 = $(NAME7).o
OBJS_WYKLUCZANIE = $(WYKLUCZANIE).o
#OBJS_BIBLIOTEKA = $(BIBLIOTEKA).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS7 = biblioteka.c

# Pliki naglowkowe
HEADS7 = biblioteka.h

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
all: $(EXEC7) $(EXEC_WYKLUCZANIE)

$(EXEC7): $(OBJS7) $(LIBS7)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_WYKLUCZANIE): $(OBJS_WYKLUCZANIE) $(LIBS7)
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_7

run-cw_7: $(EXEC7) $(EXEC_WYKLUCZANIE)
	./$(EXEC7) ./$(EXEC_WYKLUCZANIE) $(N_PROC) $(N_CRIT) $(SEMAPHOR_NAME) $(FILE_NAME)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR};

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )