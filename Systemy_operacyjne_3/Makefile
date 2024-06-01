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
NAME3a = cw_3a
NAME3b = cw_3b
NAME3c = cw_3c
NAME3cFUN = cw_3c_fun

# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC3a = $(NAME3a).x
EXEC3b = $(NAME3b).x
EXEC3c = $(NAME3c).x
EXEC3cFUN = $(NAME3cFUN).x

# Zmienne zawierające nazwy tzw. object files.
OBJS3a = $(NAME3a).o
OBJS3b = $(NAME3b).o
OBJS3c = $(NAME3c).o
OBJS3cFUN = $(NAME3cFUN).o

# Dolaczane biblioteki (poza standardowymi). 
LIBS3  = 

# Pliki naglowkowe
HEADS3 = 

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
all: $(EXEC3a) $(EXEC3b) $(EXEC3c) $(EXEC3cFUN)


$(EXEC3a): $(OBJS3a) $(LIBS3) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC3b): $(OBJS3b) $(LIBS3) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC3c): $(OBJS3c) $(LIBS3) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC3cFUN): $(OBJS3cFUN) $(LIBS3)
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_3%

run-cw_3a: $(EXEC3a)
	./$(EXEC3a) $(ARGS)

run-cw_3b: $(EXEC3b) $(EXEC3a)
	./$(EXEC3b) $(ARGS)

run-cw_3c: $(EXEC3c) $(EXEC3cFUN) $(EXEC3a)
	./$(EXEC3c) $(ARGS)

run-cw_3c_fun: $(EXEC3cFUN)
	./$(EXEC3cFUN) $(ARGS)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
