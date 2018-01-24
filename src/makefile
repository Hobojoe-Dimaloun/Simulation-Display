IDIR =../include
CC=gcc
CFLAGS=-Wall -m64 -g -I$(IDIR) -IC:/SDL2-2.0.5/x86_64-w64-mingw32/include

ODIR=obj
LDIR =../lib

LIBS=  -lm -lmingw32  -lSDL2main -lSDL2 -LC:/SDL2-2.0.5/x86_64-w64-mingw32/lib -L$(LDIR)

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

../bin/executable: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
