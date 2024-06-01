# Plik Makefile do kompilacji i wywoływania programów.
###############################################################################

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME2 = cw_2
NAME2FUN = cw_2_fun

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC2 = $(NAME2).x
EXEC2FUN = $(NAME2FUN).x

# Zmienne zawierające nazwy tzw. object files.
OBJS2 = $(NAME2).o
OBJS2FUN = $(NAME2FUN).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS2  = 

# Pliki naglowkowe
HEADS2 = 

# Opcje kompilatora i linkera
CFLAGS = -Wall -std=c99 -pedantic -O
LFLAGS = -Wall -std=c99 -pedantic -O

# Włączenie biblioteki matematycznej
LDLIBS = -lm

# Kompilator i linker
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC2) $(EXEC2FUN)


$(EXEC2): $(OBJS2) $(LIBS2) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC2FUN): $(OBJS2FUN) $(LIBS2) 
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_2

run-cw_2: $(EXEC2) $(EXEC2FUN)
	./$(EXEC2) ./$(EXEC2FUN) ./$(EXEC2FUN)

run-cw_2_fun: $(EXEC2FUN)
	./$(EXEC2FUN)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )

