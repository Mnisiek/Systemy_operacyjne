# Plik Makefile do kompilacji i wywoływania programów.
###############################################################################

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Zmienne przechowujące domyślne wartości argumentów wywołania.
N_PROC = 5
N_CRIT = 3
SEMAPHOR_NAME = semafor
FILE_NAME = numer.txt

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME6 = cw_6
WYKLUCZANIE = cw_6_wykluczanie
#BIBLIOTEKA = biblioteka.c

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC6 = $(NAME6).x
EXEC_WYKLUCZANIE = $(WYKLUCZANIE).x
#EXEC_BIBLIOTEKA = $(BIBLIOTEKA).x

# Zmienne zawierające nazwy tzw. object files.
OBJS6 = $(NAME6).o
OBJS_WYKLUCZANIE = $(WYKLUCZANIE).o
#OBJS_BIBLIOTEKA = $(BIBLIOTEKA).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS6 = biblioteka.o

# Pliki naglowkowe
HEADS6 = biblioteka.h

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
all: $(EXEC6) $(EXEC_WYKLUCZANIE)

$(OBJS6) $(OBJS_WYKLUCZANIE): $(HEADS6)

$(EXEC6): $(OBJS6) $(LIBS6)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_WYKLUCZANIE): $(OBJS_WYKLUCZANIE) $(LIBS6)
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_6

run-cw_6: $(EXEC6) $(EXEC_WYKLUCZANIE)
	./$(EXEC6) ./$(EXEC_WYKLUCZANIE) $(N_PROC) $(N_CRIT) $(SEMAPHOR_NAME) $(FILE_NAME)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR};

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )