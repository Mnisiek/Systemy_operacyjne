# Plik Makefile do kompilacji i wywoływania programów.

# Rozszerzenia plików, które będą używane w programie.
.SUFFIXES: .c .cpp .o .x .h

# Zmienna DIR przechowuje nazwę bieżącego katalogu.
DIR = $(notdir $(CURDIR))

# Zmienna SYSNAME przechowuje nazwę systemu operacyjnego.
SYSNAME = $(shell uname -s)

# Zmienne przechowujące rdzenie nazw plików, w których będą zawarte
# kody poszczególnych programów.
NAME1a = cw_1a
NAME1b = cw_1b
NAME1c = cw_1c
NAME1d = cw_1d
NAME1e = cw_1e


# Zmienne zawierające nazwy poszczególnych programów wykonawczych.
EXEC1a = $(NAME1a).x
EXEC1b = $(NAME1b).x
EXEC1c = $(NAME1c).x
EXEC1d = $(NAME1d).x
EXEC1e = $(NAME1e).x


# Zmienne zawierające nazwy tzw. object files.
OBJS1a = $(NAME1a).o
OBJS1b = $(NAME1b).o
OBJS1c = $(NAME1c).o
OBJS1d = $(NAME1d).o
OBJS1e = $(NAME1e).o


# Dolaczane biblioteki (poza standardowymi). 
LIBS1  = 

# Pliki naglowkowe
HEADS1 = 

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
all: $(EXEC1a) $(EXEC1b) $(EXEC1c) $(EXEC1d) $(EXEC1e)


$(EXEC1a): $(OBJS1a) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC1b): $(OBJS1b) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC1c): $(OBJS1c) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC1d): $(OBJS1d) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC1e): $(OBJS1e) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run-cw_1%

run-cw_1a: $(EXEC1a)
	./$(EXEC1a)

run-cw_1b: $(EXEC1b)
	./$(EXEC1b)

run-cw_1c: $(EXEC1c)
	./$(EXEC1c)

runcw_1d: $(EXEC1d)
	./$(EXEC1d)

run-cw_1e: $(EXEC1e)
	./$(EXEC1e)



$(NAME1a): $(EXEC1a)
$(NAME1b): $(EXEC1b)
$(NAME1c): $(EXEC1c)
$(NAME1d): $(EXEC1d)
$(NAME1e): $(EXEC1e)

# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )

