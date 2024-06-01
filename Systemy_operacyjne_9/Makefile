#
# Plik Makefile do kompilacji, linkowania i uruchamiania programu
#
#=========================================================================

#------------------------
DIR = `basename $(CURDIR)`

#------------------------
NAME = cw_9_2

#----------------------
EXEC = $(NAME).x
OBJS = $(NAME).o

# Zmienne przechowujące domyślne wartości argumentów
N_THREADS = 5
N_LOOPS = 3

#############################
CFLAGS = -Wall -pedantic
LFLAGS = -Wall -pedantic

# Kompilator i linker
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<


.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJS)
	 $(LD) -o $@ $(LDFLAGS) $^ -lpthread

.PHONY: run-cw_9
run-cw_9: $(EXEC)
	./$(EXEC) $(N_THREADS) $(N_LOOPS)


# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR};

# Archiwizacja i kompresja
###############################################################################
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )