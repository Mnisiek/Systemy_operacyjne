#==============================================================================
#                   Przykladowy plik Makefile
#==============================================================================
# make all, hello.x, mcint.x, run1, run2s, run2d1, run2d2, clean, tar
#==============================================================================
# Objasnienia:
#------------------------------------------------------------------------------
# Jezeli nie chcemy by make wykonal okreslone dzialania, a jedynie wypisal
# na ekranie odpowiednie instrukcje, to wywolujemy go z opcja -n, np.
# make -n all
#------------------------------------------------------------------------------
# Aby byc pewnym, ze wykonywane sa polecenia zdefiniowane w pliku Makefile,
# a nie wbudowane polecenia programu make, uruchamiac make z opcja -r, np.
# make -r run1          
#------------------------------------------------------------------------------
# Wszystko co jest po znaku # do konca linii jest traktowane jako komentarz.
#------------------------------------------------------------------------------
# Jezeli uzywamy rozszerzen w nazwach plikow, ktore nie znajduja sie na
# liscie parametrow wewnetrznych make (mozna je sprawdzic wykonujac: make -p),
# to nalezy je umiescic po slowie .SUFFIXES:, np.:
.SUFFIXES: .c .cpp .o .x .h
#------------------------------------------------------------------------------
# Ponizej przyklady definiowania zmiennych (nie musza byc oznaczane wielkimi
# literami, ale moze byc to wygodne, np.latwiej je odroznic od reszty).
# Do zmiennej ZMIENNA sie przez $(ZMIENNA)
#------------------------------------------------------------------------------
# Nazwa katalogu (moze sie przydac); wstawiamy wynik funkcji wbudowanej notdir, 
# ktora z nazwy sciezkowej wybiera wlasciwa nazwe katalogu lub pliku
# (po ostatnim /), natomiast funkcja wbudowana dir wybiera sciezke do danego
# pliku czy katalogu (przed ostatnim /).
DIR = $(notdir $(CURDIR))
# Nazwa systemu operacyjnego (jadra) - moze byc przydatna dla opcji/komend
# zaleznych od systemu. Wbudowana funkcja shell wykonuje w powloce wystepujace
# po niej polecenie, tu: uname -s (zwraca nazwe jadra systemu operacyjnego).
SYSNAME = $(shell uname -s)
# Nazwy, ktorych bedziemy uzywac, tu: rdzenie nazw plikow
# UWAGA: Nie wstawiac spacji po przypisywanej nazwie, bo zostana do niej dodane!
NAME1 = hello
NAME2 = mcint
NAME3 = funkcja
# Program wykonawczy: dodajemy rozszerzenie .x, ale moze byc dowolne.
EXEC1  = $(NAME1).x  
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1).o
# Dolaczane biblioteki (poza standardowymi). 
# Makra moga byc puste, np. przewidziane na przyszlosc.
LIBS1  = 
# Pliki naglowkowe
HEADS1 = 
#------------------------------------------------------------------------------
# Analogicznie dla drugiego programu (dla biblioteki statycznej i dzielonej)
EXEC2s  = $(NAME2)s.x
EXEC2d1  = $(NAME2)d1.x
EXEC2d2  = $(NAME2)d2.x
OBJS2  = $(NAME2).o 
OBJS3  = $(NAME3).o
# Dla drugiego programu utworzymy biblioteke statyczna oraz dzielona
LIB_NAME = MojeFunkcje
# Katalog dla bibliotek
LIB_DIR = ./lib
# Biblioteka statyczna (static library)
LIB_STAT = lib$(LIB_NAME).a
# Biblioteka dzielona linkowana dynamicznie - nazwa zalezy od systemu:
# Linux - rozszerzenie: .so
# macOS - rozszerzenie: .dylib
# Dyrektywa warunkowa sprawdzajaca, czy system nazywa sie Linux
# i wykonujaca okreslone polecenie.
ifeq ($(SYSNAME), Linux)
  LIB_SHAR = lib$(LIB_NAME).so
endif
# macOS - nazwa jadra to Darwin
ifeq ($(SYSNAME), Darwin)
  LIB_SHAR = lib$(LIB_NAME).dylib
endif
# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS3)
# Pliki naglowkowe
HEADS2 = 
HEADS3 = $(NAME3).h
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O
# Biblioteki linkera zainstalowane w systemie 
# (tu: biblioteka matematyczna libm)
LDLIBS = -lm
# Kompilator i linker (czesto ta sama nazwa)
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow 
ARFLAGS = rsv
###############################################################################
# Ponizej przyklady definiowania regul zaleznosci
###############################################################################
# Tzw. regula wzorcowa - mowiaca jak pliki z rozszerzeniem .o otrzymac 
# z plikow o rozszerzeniach .c i .h. 
# To co jest po lewej stronie dwukropka nazywa sie "cel", a to co jest po
# prawej - "prerekwizyty" (tzn. to od czego zalezy "cel"). 
# Najpierw make sprawdza czy plik celowy (tu: odpowiedni plik z rozszerzeniem 
# .o) jest aktualny (tzn. nowszy od odpowiednich plikow .c i .h) - jezeli nie, 
# to wykonuje komende ponizej (tutaj: kompilacja). 
# Znak % oznacza wzorzec, tzn. dowolny ciag znakow, ktore tworza rdzen nazwy 
# pliku. 
# Zmienna wewnetrzna $< oznacza pierwszy prerekwizyt, tu: nazwe odpowiedniego 
# pliku z rozszerzeniem .c, ktory nalezy uaktualnic aby otrzymac plik .o. 
# Uwaga: Komenda zaczyna sie od znaku tabulacji (niewidocznego).
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.c
	$(CO) $(CFLAGS) -c $<
###############################################################################
# Regula zaleznosci - uaktualnij pliki po prawej stronie dwukropka.
# Na ogol zakladamy, ze w danym katalogu nie ma pliku o nazwie all,
# tzn. ze dana regula zawsze sie wykona. 
# Gdyby plik o takiej nazwie przypadkowo znalazl sie w danym katalogu,
# to mozna wymusic bezwzgledne wykonanie sie ponizszej reguly wpisujac
# nazwe celu po slowie .PHONY:, np.
.PHONY: all
all: $(EXEC1) $(EXEC2s) $(EXEC2d1) $(EXEC2d2) 
###############################################################################
# Jawna regula zaleznosci - najpierw uaktualnij pliki po prawej stronie 
# dwukropka, a potem wykonaj komende, ktora jest w nastepnej linijce (tu:
# wykonaj linkowanie). 
# Zmienna wewnetrzna $@ oznacza cel, tzn. obiekt, ktory jest po lewej stronie 
# dwukropka, tu: $(EXEC1);  a zmienna $^ oznacza wszystkie prerekwizyty, tzn.
# obiekty stojace po prawej stronie dwukropka (tutaj wszystkie pliki .o, ktore
# powinny byc zlinkowane). 
$(EXEC1): $(OBJS1) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
###############################################################################
# Aby zapewnic bezwzgledne wykonanie niezaleznie czy istnieja pliki o danych
# nazwach (podobie jak dla all). Jezeli jestesmy pewni, ze plikow o takich
# nazwach nie bedzie w danym katalogu, to mozemy pominac ponizsza deklaracje.
.PHONY: run1
###############################################################################
# Uruchom program $(EXEC1); najpierw sprawdz, czy kod wykonawczy jest aktualny
run1: $(EXEC1)
	./$(EXEC1)
#==============================================================================
#============================  PROGRAM 2 ======================================
#==============================================================================
# Tworzenie bibliteki statycznej dla drugiego programu przy pomocy 
# programu archiwizujacego. Zmienna automatyczne $? oznacza wszystkie 
# prerekwizyty, ktore sa nowsze niz cel (dzieki temu tylko zmodyfikowane
# pliki beda wstawiane do biblioteki-archiwum). Nastepnie tworzony jest
# katalog dla biblioteki (jezeli nie istnieje) i do niego przenoszona jest
# stworzona biblioteka.
$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)
###############################################################################
# Tworzenie biblioteki dzielonej (shared library)
$(LIB_SHAR): $(LIB_OBJS)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS)
endif
# macOS - opcje linkera: -dynamiclib,  -install_name @rpath/$(LIB_DIR)/$@
ifeq ($(SYSNAME), Darwin)
	$(LD) -dynamiclib -o $@ $(LFLAGS) $(LIB_OBJS) -install_name @rpath/$(LIB_DIR)/$@
endif
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)
###############################################################################
# Regula zaleznosci dla pliku wykonawczego programu nr 2 z biblioteka statyczna
# -> po -L podajemy sciezke do biblioteki, a po -l jej nazwe bez poczatkowego 
#    lib i koncowego .a 
$(EXEC2s): $(OBJS2) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)
###############################################################################
#  Plik wykonawczy z biblioteka dzielona - wersja 1
#  -> informacja o lokalizacji bibliteki przekazywana jest przez opcje linkera:
#     -Wl,-R $(LIB_DIR)  (zamiast -R mozna uzyc -rpath) dla Linuksa.       
$(EXEC2d1): $(OBJS2) $(LIB_SHAR)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif
#-------------------------------------------------------------------------------------------------
#  Plik wykonawczy z biblioteka dzielona - wersja 2
#  -> informacja o lokalizacji biblioteki nie jest przekazywana przez opcje 
#     linkera, lecz na etapie wykonania przez zmienna srodowiskowa:
#     LD_LIBRARY_PATH (patrz ponizej na przyklad uruchomienia programu).
$(EXEC2d2): $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)
###############################################################################
# Dodatkowe reguly zaleznosci plikow objectowych od naglowkowych, ktorych
# nie uwglednia regula wzorcowa (powyzej).
###############################################################################
$(OBJS2): $(HEADS3)
###############################################################################
.PHONY: run2s run2d1 run2d2
###############################################################################
# Uruchom program, ale najpierw sprawdz czy kod wykonawczy jest aktualny. 
# 1) z biblioteka statyczna
run2s: $(EXEC2s)
	./$(EXEC2s)
# 2.1) z biblioteka dzielona - wersja 1
run2d1: $(EXEC2d1)
	./$(EXEC2d1)
# 2.2) z biblioteka dzielona - wersja 2 (znak \ oznacza kontynuacje w nastepnej
#      linii) -> do zmiennej srodowiskowej LD_LIBRARY_PATH trzeba dodac sciezke
#      do katalogu z nowa biblioteka dzielona i uruchomic program w tej samej
#      powloce (dlatego znak kontynuacji \, bo bez tego make wykonujac komende
#      w danej linii tworzy nowa powloke, a nastepnie ja zamyka itd.).
#      Robiac to samo bezposrednio z powloki (tzn. bez make) wystarczy raz
#      wykonac pierwsza linijke (bez znakow ; i \), a potem mozna wielokrotnie
#      wykonywac program w tej powloce.     
run2d2: $(EXEC2d2)
# Linux
ifeq ($(SYSNAME), Linux)
	export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC2d2)
endif
# macOS - tu odpowiednia zmienna srodowiskowa to: DYLD_LIBRARY_PATH
ifeq ($(SYSNAME), Darwin)
	export DYLD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC2d2)
endif
###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
