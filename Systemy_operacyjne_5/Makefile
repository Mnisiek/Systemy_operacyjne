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

# Domyślna nazwa potoku nazwanego (FIFO).
FIFO_NAME = mojfifo

# Porównanie zawartości plików
compare:
	diff -s $(WEJSCIE) $(WYJSCIE)

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME5a = cw_5a
KONSUM = konsument
PRODUC = producent

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC5a = $(NAME5a).x
EXEC_PRODUC = $(PRODUC).x
EXEC_KONSUM = $(KONSUM).x

# Zmienne zawierające nazwy tzw. object files.
OBJS5a = $(NAME5a).o
OBJS_PRODUC = $(PRODUC).o
OBJS_KONSUM = $(KONSUM).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS5 = 

# Pliki naglowkowe
HEADS5 = 

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
all: $(EXEC5a) $(EXEC_PRODUC) $(EXEC_KONSUM)

$(EXEC5a): $(OBJS5a) $(LIBS5)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_PRODUC): $(OBJS_PRODUC) $(LIBS5)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC_KONSUM): $(OBJS_KONSUM) $(LIBS5)
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_5a%

run-cw_5a: $(EXEC5a) $(EXEC_PRODUC) $(EXEC_KONSUM)
	./$(EXEC5a) $(WEJSCIE) $(WYJSCIE) $(FIFO_NAME) ./$(EXEC_PRODUC) ./$(EXEC_KONSUM)

run-producent: $(EXEC_PRODUC)
	./$(EXEC_PRODUC) $(FIFO_NAME) $(WEJSCIE)

run-konsument: $(EXEC_KONSUM)
	./$(EXEC_KONSUM) $(FIFO_NAME) $(WYJSCIE)

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